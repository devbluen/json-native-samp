
static stock bool:SAFE_INDEX(const str[], idx) {
    return ((idx) >= 0 && (idx) < strlen(str));
}

stock bool:json_has_key(const jsonStr[], const key[]) {
    new keyWithQuotes[64];
    format(keyWithQuotes, sizeof(keyWithQuotes), "\"%s\":", key);

    return (strfind(jsonStr, keyWithQuotes, true) != -1);
}

stock bool:json_string(const jsonStr[], const key[], value[], const size = sizeof(value)) {
    if (!json_has_key(jsonStr, key)) return false;

    new keyWithQuotes[64];
    format(keyWithQuotes, sizeof(keyWithQuotes), "\"%s\":", key);

    new pos1 = strfind(jsonStr, keyWithQuotes, true);
    if (pos1 == -1) {
        value[0] = '\0';
        return false;
    }

    pos1 += strlen(keyWithQuotes);
    while (SAFE_INDEX(jsonStr, pos1) && (jsonStr[pos1] == ' ' || jsonStr[pos1] == '\t')) pos1++;

    if (!SAFE_INDEX(jsonStr, pos1)) return false;

    new pos2 = pos1;

    if (jsonStr[pos1] == '\"') {
        pos1++;
        pos2 = pos1;
    }

    while (SAFE_INDEX(jsonStr, pos2)) {
        // Verifica se é um delimitador e se está escapado
        if (jsonStr[pos2] == ',' || jsonStr[pos2] == '}' || jsonStr[pos2] == ']') {
            new backslashes = 0;
            for (new i = pos2 - 1; i >= 0 && jsonStr[i] == '\\'; i--) {
                backslashes++;
            }

            if ((backslashes % 2) == 0) { // Se número de '\' for par, o caractere NÃO está escapado (fim da string)
                break;
            }
            // Se ímpar, o caractere está escapado, então ele faz parte da string
        }
        pos2++;
    }

    if (!SAFE_INDEX(jsonStr, pos2) || pos2 <= pos1) {
        value[0] = '\0';
        return false;
    }

    // Extrai o valor corretamente
    strmid(value, jsonStr, pos1, pos2, size);

    // Se o último caractere for uma aspas, remove
    new lastChar = strlen(value) - 1;
    if (lastChar >= 0 && value[lastChar] == '\"') {
        value[lastChar] = '\0';
    }

    return true;
}

stock bool:json_bool(const jsonStr[], const key[], &value) {
    if (!json_has_key(jsonStr, key)) return false;

    new stringValue[10]; 
    json_string(jsonStr, key, stringValue, sizeof stringValue);

    if (strcmp(stringValue, "true", true) == 0) {
        value = true;
    } else if (strcmp(stringValue, "false", true) == 0) {
        value = false;
    } else {
        return false; // Valor não é um booleano válido
    }
    return true;
}

stock bool:json_int(const jsonStr[], const key[], &value) {
    if(!json_has_key(jsonStr, key))
        return false;

    new stringValue[64];
    json_string(jsonStr, key, stringValue, sizeof stringValue);
    value = strval(stringValue);
    return true;
}

stock bool:json_float(const jsonStr[], const key[], &Float:value) {
    if(!json_has_key(jsonStr, key))
        return false;

    new stringValue[64];
    json_string(jsonStr, key, stringValue, sizeof stringValue); 
    value = floatstr(stringValue);
    return true;
}

stock json_array_length(const jsonStr[], const key[]) {
    new keyWithQuotes[64];
    format(keyWithQuotes, sizeof(keyWithQuotes), "\"%s\":", key);

    new pos1 = strfind(jsonStr, keyWithQuotes, true);
    if (pos1 == -1) return 0; // Array não encontrado

    pos1 += strlen(keyWithQuotes);
    while (SAFE_INDEX(jsonStr, pos1) && (jsonStr[pos1] == ' ' || jsonStr[pos1] == '\t')) pos1++;

    if (!SAFE_INDEX(jsonStr, pos1) || jsonStr[pos1] != '[') return 0; // Não é um array JSON

    pos1++; // Avança para dentro do array
    new count = 0;

    while (SAFE_INDEX(jsonStr, pos1) && jsonStr[pos1] != ']' && jsonStr[pos1] != '\0') {
        while (SAFE_INDEX(jsonStr, pos1) && (jsonStr[pos1] == ' ' || jsonStr[pos1] == '\t' || jsonStr[pos1] == ',')) pos1++;

        if (!SAFE_INDEX(jsonStr, pos1)) return count; // Evita acesso inválido

        if (jsonStr[pos1] == '{') { // Apenas conta objetos, ignorando valores soltos
            count++;
        }

        while (SAFE_INDEX(jsonStr, pos1) && jsonStr[pos1] != ',' && jsonStr[pos1] != ']' && jsonStr[pos1] != '\0') pos1++;
    }
    return count; // Retorna número correto de objetos no array
}

stock json_array_get(const jsonStr[], const key[], index, value[], const size = sizeof(value)) {
    new keyWithQuotes[64];
    format(keyWithQuotes, sizeof(keyWithQuotes), "\"%s\":", key);

    new pos1 = strfind(jsonStr, keyWithQuotes, true);
    if (pos1 == -1) return 0; // Array não encontrado

    pos1 += strlen(keyWithQuotes);
    while (SAFE_INDEX(jsonStr, pos1) && (jsonStr[pos1] == ' ' || jsonStr[pos1] == '\t')) pos1++;

    if (!SAFE_INDEX(jsonStr, pos1) || jsonStr[pos1] != '[') return 0; // Não é um array JSON

    pos1++; // Avança para dentro do array
    new count = 0, pos2;

    while (SAFE_INDEX(jsonStr, pos1) && jsonStr[pos1] != ']' && jsonStr[pos1] != '\0') {
        while (SAFE_INDEX(jsonStr, pos1) && (jsonStr[pos1] == ' ' || jsonStr[pos1] == '\t' || jsonStr[pos1] == ',')) pos1++;

        if (!SAFE_INDEX(jsonStr, pos1)) return 0; // Verificação de segurança

        if (jsonStr[pos1] == '{') { // Caso seja um objeto JSON
            pos2 = strfind(jsonStr, "}", true, pos1);
            if (!SAFE_INDEX(jsonStr, pos2) || pos2 <= pos1) return 0; // Objeto inválido

            pos2++; // Inclui o fechamento correto do objeto JSON

        } else { // Caso seja um valor direto
            if (jsonStr[pos1] == '\"') {
                pos1++;
                pos2 = strfind(jsonStr, "\"", true, pos1);
            } else {
                pos2 = pos1;
                while (SAFE_INDEX(jsonStr, pos2) && jsonStr[pos2] != ',' && jsonStr[pos2] != ']' && jsonStr[pos2] != '\0') pos2++;
            }
        }

        if (count == index) { // Encontrou o valor desejado
            if (!SAFE_INDEX(jsonStr, pos2) || pos2 <= pos1) {
                value[0] = '\0';
                return 0;
            }

            strmid(value, jsonStr, pos1, pos2, size);
            return 1;
        }

        count++;
        pos1 = pos2 + 1;
    }

    return 0; // Índice fora do alcance
}

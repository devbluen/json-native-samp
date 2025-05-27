
stock json_string(const jsonStr[], const key[], value[], const size = sizeof(value)) {
    
    new 
        keyWithQuotes[64];
    format(keyWithQuotes, sizeof(keyWithQuotes), "\"%s\":", key); 

    new 
        pos1 = strfind(jsonStr, keyWithQuotes, true); 

    if (pos1 == -1) {
        value[0] = '\0'; 
        return;
    }

    pos1 += strlen(keyWithQuotes);
    while (jsonStr[pos1] == ' ' || jsonStr[pos1] == '\t') pos1++; 
    new pos2 = pos1;
    if (jsonStr[pos1] == '\"') {
        pos1++;
        pos2 = strfind(jsonStr, "\"", true, pos1);
    } else {
        while (jsonStr[pos2] != ',' && jsonStr[pos2] != '}' && jsonStr[pos2] != '\0') pos2++;
    }
    if (pos2 == -1 || pos2 <= pos1) {
        value[0] = '\0'; 
        return;
    }
    strmid(value, jsonStr, pos1, pos2, size);
}

stock json_array_length(const jsonStr[], const key[]) {
    new keyWithQuotes[64];
    format(keyWithQuotes, sizeof(keyWithQuotes), "\"%s\":", key);

    new pos1 = strfind(jsonStr, keyWithQuotes, true);
    if (pos1 == -1) return 0; // Array não encontrado

    pos1 += strlen(keyWithQuotes);
    while (jsonStr[pos1] == ' ' || jsonStr[pos1] == '\t') pos1++;

    if (jsonStr[pos1] != '[') return 0; // Não é um array JSON

    pos1++; // Avança para dentro do array
    new count = 0;

    while (jsonStr[pos1] != ']' && jsonStr[pos1] != '\0') {
        while (jsonStr[pos1] == ' ' || jsonStr[pos1] == '\t' || jsonStr[pos1] == ',') pos1++;

        if (jsonStr[pos1] == '{') { // Apenas conta objetos, ignorando valores soltos
            count++;
        }

        while (jsonStr[pos1] != ',' && jsonStr[pos1] != ']' && jsonStr[pos1] != '\0') pos1++;
    }

    return count; // Retorna número correto de objetos no array
}

stock json_array_get(const jsonStr[], const key[], index, value[], const size = sizeof(value)) {
    new keyWithQuotes[64];
    format(keyWithQuotes, sizeof(keyWithQuotes), "\"%s\":", key);

    new pos1 = strfind(jsonStr, keyWithQuotes, true);
    if (pos1 == -1) return 0; // Array não encontrado

    pos1 += strlen(keyWithQuotes);
    while (jsonStr[pos1] == ' ' || jsonStr[pos1] == '\t') pos1++;

    if (jsonStr[pos1] != '[') return 0; // Não é um array JSON

    pos1++; // Avança para dentro do array
    new count = 0, pos2;

    while (jsonStr[pos1] != ']' && jsonStr[pos1] != '\0') {
        while (jsonStr[pos1] == ' ' || jsonStr[pos1] == '\t' || jsonStr[pos1] == ',') pos1++;

        if (jsonStr[pos1] == '{') { // Caso seja um objeto JSON
            pos2 = strfind(jsonStr, "}", true, pos1);
            if (pos2 == -1 || pos2 <= pos1) return 0; // Objeto inválido

        } else { // Caso seja um valor direto
            if (jsonStr[pos1] == '\"') {
                pos1++;
                pos2 = strfind(jsonStr, "\"", true, pos1);
            } else {
                pos2 = pos1;
                while (jsonStr[pos2] != ',' && jsonStr[pos2] != ']' && jsonStr[pos2] != '\0') pos2++;
            }
        }

        if (count == index) { // Encontrou o valor desejado
            if (pos2 == -1 || pos2 <= pos1) {
                value[0] = '\0';
                return 0;
            }

            strmid(value, jsonStr, pos1, pos2 + (jsonStr[pos1 - 1] == '{' ? 1 : 0), size);
            return 1;
        }

        count++;
        pos1 = pos2 + 1;
    }

    return 0; // Índice fora do alcance
}

stock json_int(const jsonStr[], const key[], &value) {
    new stringValue[64];
    json_string(jsonStr, key, stringValue, sizeof stringValue);
    value = strval(stringValue);
}

stock json_float(const jsonStr[], const key[], &Float:value) {
    new stringValue[64];
    json_string(jsonStr, key, stringValue, sizeof stringValue); 
    value = floatstr(stringValue);
}

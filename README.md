# json-native-samp
This include will help you read JSON without needing plugins. For now this include does not create the JSON, it can only read it, however I will soon be bringing this logic

# 🪶 How to use examples
### Json Example
```json
{
  "name": "samp",
  "age": 7,
  "height": 120.0,
  "arrayWithObject": [
    {
      "discord": "samp-lab",
      "players": 22
    },
    {
      "discord": "central studios",
      "players": 45  
    }
  ],
}
```

### Get with above Json Example
```pawn
new name[...]; 
json_string(stringJson, "name", name); -> return -> samp

new age;
json_int(stringJson, "age", age); -> return -> 7

new Float:height;
json_float(stringJson, "height", height); -> return  -> 120.0

new field[...];
for(new i = 0; i < json_array_length(stringJson, "arrayWithObject"); i++) {
    json_array_get(stringJson, "arrayWithObject", i, field);

    // Now you can use the same methods above
    new discord[...];
    json_string(field, "discord", discord); -> return -> "samp-lab" if two index "central studios"
  
    new players;
    json_int(field, "players", players); -> return -> 22 if two index 45
}
```

# 🚀 Natives
```pawn
bool:json_has_key(const jsonStr[], const key[]);
bool:json_string(const jsonStr[], const key[], value[], const size = sizeof(value))
bool:json_int(const jsonStr[], const key[], &value)
bool:json_float(const jsonStr[], const key[], &Float:value)
bool:json_bool(const jsonStr[], const key[], &:value)
json_array_length(const jsonStr[], const key[])
bool:json_array_get(const jsonStr[], const key[], index, value[], const size = sizeof(value))
```

# 🤍 Thanks
- Microsoft Copilot (Helped with array creation logic and testing)

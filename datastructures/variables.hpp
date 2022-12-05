#pragma once

class Variables
{
private:
    int ncid;
    char* vars[];
    int* var_ids;
    void find_var_ids();
public:
    Variables(int file_id, char** vars);
    int* get_var_ids(char** var_names);
    int get_var_id(char* var_name);
    Variables();
};
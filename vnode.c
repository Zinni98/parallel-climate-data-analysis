#include <stdio.h>
#include <stddef.h>
#include <string.h>
#include <netcdf.h>

int get_file_id(char* filename)
{
    /**
     * @param filename path to the netcdf file
     * @returns id of the file
    */
    int ncid;
    int status = nc_open(filename, NC_NOWRITE, &ncid);
    if(status != NC_NOERR)
        printf("Error during the file opening\n");
    return ncid;
}

void get_var_ids(int ncid, char** var_names, int var_ids[], int count)
{
    /**
     * @brief returns var_id of vnod
     * @param ncid id of netcdf file
     * @param var_names array of netcdf variable names for which ids should be ectracted
     * @param var_ids array used to store the result
     * @param count number of elements in var_names
     * @returns error status, if no errors occur, constant NC_NOERR is returned
    */
    int status;
    for(int i=0; i<count; i++)
    {
        int var_id;
        status = nc_inq_varid(ncid, var_names[i], &var_id);
        if(status != NC_NOERR)
            break;
        else
            var_ids[i] = var_id;
    }
    if (status != NC_NOERR)
        printf("Error during var_id retrieval\n");
    else   
        printf("File read succesfully");
    //return status;
}

bool check_var(char **options, chat *var_name){
    for(int i=0, i<3, i++){
        if(!strcmp(options[i], *var_name)){
            break;
        }else{
            flag=1;
        }
    }
    if(flag){
        printf("Undefined variable name as input")
    }

    return flag
}
void read_velocity(int ncid, char **options, char* var_name , const int start_idxs[], const int count_idxs[], const int stride_steps[], float target_buffer[][][]){

    // support vars
    bool flag=0;
    int var_id;
    int status_varid;
    int status_buffer;

    // starting point, end point and stride
    const size_t startv[3] = {start_idxs[0], start_idxs[1], start_idxs[2]};
    const size_t countv[3] = {count_idxs[0], count_idxs[1], count_idxs[2]};
    const ptrdiff_t stridev[3] = {stride_steps[0], stride_steps[1], stride_steps[2]};

    flag = check_var(options, var_name)
    if(!flag){
        status_varid = nc_inq_varid(ncid, *var_name, &var_id);
        if(status_varid==NC_NOERR){
            // writing all "var_name" related data from file with id "ncid"
            // from "startv" to "countv" with step "stridev" (dimension-wise) to "target buffer"
            status_buffer = nc_get_vars_float(ncid, *var_name, &startv[0], &countv[0], &stridev[0], &target_buffer[0][0][0]);
            if (status != NC_NOERR){
                printf("Error during data retrieval\n");
                return 1;
            }
        }else{
            printf("Detected error while readin %s id", *var_name);
        }
    }    
    return status_buffer
}

double* compute_maximum(char *var_name, float ***matrix, int rows, int columns, int dpth){
    // assuming valid names
    float maximums[][][] = {};
    char* dim_names = {"time", "depth", "vnod"};
    
    if(strcmp(*var_name, dim_names[1]){
        printf("Depth reduction");

        for(int i=0; i<rows; i++){
            for(int j=0; j<dpth;j++){
                maximums[i][j] = matrix[i][0][j]
            }
        }

        for(int depth=1; depth<columns; depth++){
            for(int time=0; time<rows; time++){
                for(int node2=0; node2<depth; node2++){
                    if(matrix[time][depth][node2] > maximums[time][node2]){
                        maximums[time][node2] = matrix[time][depth][node2]
                    }
                }
            }
        }
    }
    return maximums
}

int main(int argc, char**argv)
{
    // var declaration
    char path[] = "/shares/HPC4DataScience/FESOM2/vnod.fesom.2010.nc";
    char* vars[] = {"nz1", "time", "vnod"};
    int var_ids[3];
    float vnod[12][69][8852366];
    
    // get file id
    int ncid = get_file_id(path);
    // get var ids
    get_var_ids(ncid, vars, var_ids, 3);
    
    
    const int startv[3] = {0, 0, 0};
    const int countv[3] = {1, 1, 200};
    const int stridev[3] = {1, 1, 1};
    status = nc_get_vars_float(ncid, var_ids[2], &startv[0], &countv[0], &stridev[0], &vnod[0][0][0]);
    
    read_velocity(ncid, vars, vars[2], &startv[0], &countv[0], &stridev[0], &vnod[0][0][0])

    return 0;
}

#include <stdio.h>
#include <stddef.h>
#include <netcdf.h>

/**
 * TODO: Modify get_var_ids to return an array of structs containing var_name + var_id
 * */

int get_file_id(char* filename)
{
    /**
     * @param filename path to the netcdf file
     * @returns id of the file
    */
    int ncid;
    int status = nc_open(filename, NC_NOWRITE, &ncid);
    if(status != NC_NOERR)
        printf("Errror during the file opening\n");
    return ncid;
}
int get_var_ids(int ncid, char** var_names, int var_ids[], int count)
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
    return status;
}

int main(int argc, char**argv)
{
    char path[] = "/shares/HPC4DataScience/FESOM2/vnod.fesom.2010.nc";
    int ncid = get_file_id(path);
    char* vars[] = {"nz1", "time", "vnod"};
    int var_ids[3];
    int status = get_var_ids(ncid, vars, var_ids, 3);
    
    if (status != NC_NOERR)
        printf("Error during var_id retrieval\n");
    
    const size_t startv[3] = {0, 0, 0};
    const size_t countv[3] = {1, 1, 200};
    const ptrdiff_t stridev[3] = {1, 1, 1};
    float vnod[12][69][8852366];
    status = nc_get_vars_float(ncid, var_ids[2], &startv[0], &countv[0], &stridev[0], &vnod[0][0][0]);
    if (status != NC_NOERR)
    {
        printf("Error during lon var retrieval\n");
        return 1;
    }
    else
    {
        printf("Printing first 200 coordinates:\n");
        for(int i=0; i<200; i++)
        {
            printf("vnod: %f\n", vnod[0][0][i]);
        }
    }

    return 0;
}
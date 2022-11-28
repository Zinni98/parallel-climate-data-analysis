#include <iostream>
#include <netcdf.h>
#include <string>

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
        std::cout<<"Errror during the file opening"<<std::endl;
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
    char path[] = "/shares/HPC4DataScience/FESOM2/fesom.mesh.diag.nc";
    int ncid = get_file_id(path);
    char* vars[] = {"nz", "nz1", "lon", "lat"};
    int var_ids[3];
    int status = get_var_ids(ncid, vars, var_ids, 4);
    
    if (status != NC_NOERR)
        std::cout<<"Error during var_id retrieval"<<std::endl;
    
    double lon[8852366];
    

    
    double lat[8852365];
    status = nc_get_var_double(ncid, var_ids[3], &lat[0]);
    if (status != NC_NOERR)
    {
        std::cout<<"Error during var retrieval"<<std::endl;
        return 1;
    }
    else
    {
        std::cout<<"Printing first 100 coordinates:"<<std::endl;
        for(int i=0; i<100; i++)
        {
            std::cout<<"lat: "<<lat[i]<<"; lon"<<lon[i]<<std::endl;
        }
    }

    return 0;
}
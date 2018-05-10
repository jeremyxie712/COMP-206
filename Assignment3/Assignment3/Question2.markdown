This is the Question 2 of the COMP206 Assignment 3.  
Jeremy Xie   
McGill ID: 260660974

# Explanation 
The `main` fucntion in **movie.c** loads preferences of users text file by calling `load_one_preference` function in **preference.c**. It reads the file by lines (*It uses global variables defined in header files*) and then calls the `LOAD_FIELD` function who has an array of function pointers `input_fcn_array` decide to point to the appropriate functions. The `DEBUG` was then used and `print_preference` was called, a function who calls the `PRINT_FIELD` who also possesses `output_fcn_array` to decide to point to the appropriate function. It calls the **distance.c** to compute the distance, lower closer, eventually it prints the best match by using `DEBUG`.

# LOAD_FIELD
The `LOAD_FIELD` function is an array of pointers to functions who was initialized by the number of fields. It contains functions like 

1.`input_string`   
2.`input_double`  
3. `input_animal`  
4. ...  

It works in the likes of which of a table who uses the passed parameter `dest` and `src` and the appropriate field from the `offsetof` and these three elements combined to select our desired field and functions from the `input_fcn_array` array of function pointers. 


# Offsets
The `field_offsets` is an array who stores `offsetof` of structures and fields. It starts from the first field to the last field in the desired order, but the struct order and actual written order might be different, thus using `offsetof` best secured us by returning the number of bytes from the beginning to the specific field we need, it helped us to avoid the scribble scrabbled text files. 


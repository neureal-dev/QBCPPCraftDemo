# QBCPPCraftDemo

project created as QB task, to avoid unnecessary github tracking

## Build Instructions

### windows:
`cd build
cmake ..
devenv /build Release QBCPPCraftDemo.sln`

### linux:
`cd build
cmake ..
make`

## Performance comparison

test: number of execution / size of the initial array

solutions:
 - original: solution provided in QB repository
 - optimized: solution with optimized lambda function
 - custom solution with strong typization and memory layout


solution\test  | 1 / 1K    | 10 / 100K | 100 / 1M
---------------|-----------|-----------|----------
original       | 7.59e-05  | 0.0076    | 0.7931 
optimized      | 2.08e-05  | 0.0023    | 0.0233
custom         | 1.37e-05  | 0.0016    | 0.0205

## Other considerations

- collection and record type intended to be flexible and adjustable 
- standard containers
- no target platform and cross platform compatibility
- delete and index search is rare operation, data is unsorted and QB* implementations focuses on performance vs stability of the delete operation
- implementation not trying to solve data consistency problem and more focusing on full collection search
- most of the queries will involve full collection scan: loop through all records and string search through record attributes

- strong typing vs weak typing provide about 30% even with optimized version of original implementation
- style of the code was combined, so that clang used to sync everything to a single style
- removed windows dependencies and non standard extensions
- tried to keep single file solution

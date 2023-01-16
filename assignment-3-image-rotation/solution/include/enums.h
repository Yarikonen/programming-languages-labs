#ifndef ENUMERATIONS
#define ENUMERATIONS
enum readStatus {
 READ_OK = 0,
 READ_INVALID_SIGNATURE,
 READ_INVALID_BITS,
 READ_INVALID_HEADER,
 READ_PADDING_ERROR,
 READ_ALLOCATION_ERROR

};
enum writeStatus {
 WRITE_OK = 0,
 WRITE_INVALID_SIGNATURE,
 WRITE_INVALID_BITS,
 WRITE_INVALID_HEADER,
 WRITE_PADDING_ERROR,
 WRITE_ALLOCATION_ERROR

};
enum headerState{
        HEADER_OK =0,
        INVALID_HEADER

};
enum openFileStatus{
        OPEN_OK=0,
        SMTH_WRONG
};
enum rotationStatus{
        ROTATION_OK =0,
        MALLOC_ERROR
};

#endif

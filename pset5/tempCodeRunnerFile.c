int powerFunction (int x, int h) {
    int i; 
    int number = 1;

    for (i = 0; i < h; ++i){
        number *= x;
    }    

    return number;
}
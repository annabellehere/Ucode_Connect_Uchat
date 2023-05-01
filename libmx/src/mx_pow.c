double mx_pow(double n, unsigned int pow) {
    double result = n;
    
    if (n == 0) return 0;
    if (n == 1) return 1;
    if (pow == 0) return 1;
    else {
        for (unsigned int i = 1; i < pow; i++) {
            result = result * n;
        }
    }
    return result;
}

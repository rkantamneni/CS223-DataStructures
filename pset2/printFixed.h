/*
 * Fixed precision printer.
 *
 * Takes a (signed) long long and formats as fixed-point decimal,
 * printing to stdout.
 *
 * Parameter number is the number to print. It can be any value
 * that can be stored in a long long.
 *
 * Parameter separator specifies character to group thousands by.
 *
 * Parameter decimalPoint specifies character for decimal point.
 *
 * Parameter precision specifies number of digits after the decimal point.
 * If precision is greater than or equal to the number of digits in
 * LONG_LONG_MAX, behavior is undefined.
 *
 * The interpretation is that number is actually number * 10^(-precision),
 * so 123 with precision 2 represents 1.23.
 *
 * Examples:
 *
 * printFixed(123456789, ',', '.', 2) prints 1,234,567.89
 * printFixed(-123456789, '.', ',', 4) prints -12.345,6789
 * printFixed(123456789, ',', '.', 12) prints 0.000123456789
 */

void
printFixed(long long number, char separator, char decimalPoint, unsigned int precision);

/*
 * This is a suggestion of implementing rather optimal Euclidean algorithm
 * of finding Greatest Common Divider.
 *
 * int gcd(int a, int b) {
 *   int t;
 *   while (b != 0) {
 *     t = b;
 *     b = a % b;
 *     a = t;
 *   }
 *   return a;
 * }
 */

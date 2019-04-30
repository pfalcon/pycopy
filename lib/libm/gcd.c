/*
 * This is a suggestion of implementing rather optimal Euclidean algorithm
 * of finding Greatest Common Divider.
 *
 * The the first one is iterative:
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
 * 
 * The second one is a version with loop-unrolling:
 *
 * int gcd(int a, int b)  
 * {  
 *   while(true)  
 *   {  
 *     a = a % b;  
 *     if(a == 0)  
 *     {  
 *       return b;  
 *     }  
 *     b = b % a;  
 *     if(b == 0)  
 *     {  
 *       return a;  
 *     }  
 *   }  
 * }
 */

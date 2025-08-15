       #ifdef __STDC_IEC_559__
            // Code that assumes IEEE 754 compliance
            #include <iostream>
            #include <limits>

            int main() {
                if (std::numeric_limits<double>::is_iec559) {
                    std::cout << "This system uses IEEE 754 for double-precision floating-point." << std::endl;
                }
                return 0;
            }
        #else
            // Code for systems that don't use IEEE 754
            std::cout << "This system might not use IEEE 754." << std::endl;
        #endif
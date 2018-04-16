//Partner Work: Project 1
//Elia Deppe & Tatiana Nikolaeva

//CMSC 441
//Professor Marron

/*
!!!NOTE (Elia)!!!
    This was created while using Codeblocks on a Windows machine. When testing the
    project I used a .exe file and a text file input on the command line. That is why
    the main uses argument std::ifstream file(argv[1]); I was unsure of how to adjust
    this to ensure it to read the correct position for the text file for machines running Linux
    or Mac or if someone wished to output to a specific file using terminal arguments.
    Please adjust it to the correct position if necessary.
!!!END NOTE!!!
*/

//Include
#include <iostream>
#include <fstream>
#include <cstdlib>
#include <time.h>
#include <string>
#include <boost/multiprecision/cpp_int.hpp>
#include <boost/random.hpp>
#include <boost/lexical_cast.hpp>
#include <chrono>

using namespace boost::multiprecision;

//used cpp_int from the boost multiprecision libraries

//class myComplex
//creates a myComplex object that houses a complex number with the real and imaginary
//  variables being cpp_int
//developed by Tatiana, with additions by Elia
class myComplex {
    public:
        cpp_int real, imag;                 //real and imag parts
        myComplex();                        //default constructor
        myComplex(const myComplex &item);   //copy constructor
        void print();                       //print function
};

//constructor
myComplex::myComplex() {
    real = 0;
    imag = 0;
}

//copy constructor
myComplex::myComplex(const myComplex &item) {
    real.assign(item.real);
    imag.assign(item.imag);
}

//print
// prints complex number in format (<real>, <imag>);
void myComplex::print() {
    std::cout << "(" << real << ", " << imag << ")\n";
}

//cmul4
//does 4 multiplies for obtaining a complex number
//added by Tatiana
void cmul4(myComplex a, myComplex b, myComplex *z);

//cmul3
//does 3 multiplies for obtaining a complex number
//added by Tatiana
void cmul3(myComplex a, myComplex b, myComplex *z);

//cmul4_list
//recursively 4 multiplies a list of complex numbers (divide and conquer)
//based off merge sort pseudo code from book
myComplex cmul4_list(std::vector<myComplex> *items, int p, int r);

//cmul3_list
//recursively 3 multiplies a list of complex numbers (divide and conquer)
//based off merge sort pseudo code from book
myComplex cmul3_list(std::vector<myComplex> *items, int p, int r);

//cmul4_list
//recursively 4 multiplies a list of complex numbers (n - 1)
//added by Elia with help from Tatiana
myComplex cmul4_list(std::vector<myComplex> *items, int i);

//cmul3_list
//recursively 3 multiplies a list of complex numbers (n - 1)
//added by Elia with help from Tatiana
myComplex cmul3_list(std::vector<myComplex> *items, int i);

//main
//
//done by Tatiana and Elia
int main(int argc, char *argv[]) {
    myComplex a, b, z;
    int i, index, len;
    clock_t t0, t;
    std::chrono::high_resolution_clock::time_point start;
    std::chrono::high_resolution_clock::time_point finish;
    std::chrono::duration<double> time_span;
    std::string line, real_string, imag_string;
    std::vector<myComplex> nums;

    //random cpp_int generator based on the mt19937
    //used for a and b when calling cmul4 and cmul3
    typedef boost::random::independent_bits_engine<boost::mt19937, 20, cpp_int> generator_type;
    generator_type gen;

    a.real.assign(gen());
    a.imag.assign(gen());
    b.real.assign(gen());
    b.imag.assign(gen());
    z.real.assign(0);
    z.imag.assign(0);

    std::cout << "Random numbers generated \na: ";
    a.print();
    std::cout << "b: ";
    b.print();

    cmul4(a, b, &z);

    std::cout << "cmul4 of a and b: ";
    z.print();

    cmul3(a, b, &z);

    std::cout << "cmul3 of a and b: ";
    z.print();


    if(argc == 1) {
        std::cout << "Please specify an input file.";
        return 0;
    }

    //!!!
    //please modify this to fit for your specific command
    //this is formatted for windows cmd line when using a .exe file
    //!!!
    std::ifstream file(argv[1]);

    if(!(file.is_open())) {
        std::cout << "Incorrect filename: please re-enter the file name." << '\n';
        return 0;
    }

    //getting each line if the file and assigning them into the nums vector
    //this read is formatted for the file to read as such:
    //(<real>, <imaginary>)\n
    //the if statement is used to catch a possible final \n on the bottom line
    //  or for any possible empty lines.
    for(i = 0; getline(file, line); i++) {
        if(line.length() > 2) {
            nums.push_back(myComplex());
            index = line.find_first_of(',');

            real_string = line.substr(1, index - 1);
            imag_string = line.substr(index + 2, line.length() - index - 3);

            nums[i].real.assign(boost::lexical_cast<cpp_int>(real_string));
            nums[i].imag.assign(boost::lexical_cast<cpp_int>(imag_string));
        }
        else {
            i--;
        }
    }

    //get length and reset z
    len = nums.size();
    z = myComplex();

    /*--- divide and conquer section ---*/
    std::cout << "--- divide and conquer section ---" << std::endl;


    //set clock values and run cmul4_list

    start = std::chrono::high_resolution_clock::now();
    z = cmul4_list(&nums, 0, len - 1);
    finish = std::chrono::high_resolution_clock::now();

    time_span = std::chrono::duration_cast<std::chrono::duration<double>>(finish - start);

    std::cout << "cmul4_list time " << time_span.count() << std::endl;
    //std::cout << "result: ";
    //z.print();

    //set clock values and run cmul3_list
    start = std::chrono::high_resolution_clock::now();
    z = cmul4_list(&nums, 0, len - 1);
    finish = std::chrono::high_resolution_clock::now();

    time_span = std::chrono::duration_cast<std::chrono::duration<double>>(finish - start);
    std::cout << "cmul3_list time " << time_span.count() << std::endl;
    //std::cout << "result: ";
    //z.print();

    /*--- n - 1 section ---*/
    std::cout << "--- n - 1 section ---" << std::endl;

    //set clock values and run cmul4_list
    t0 = clock();
    z = cmul4_list(&nums, len - 1);
    t = clock() - t0;

    std::cout << "cmul4_list time (n-1): " << ((double(t) / CLOCKS_PER_SEC)) << std::endl;
    //std::cout << "result: ";
    //z.print();

    //set clock values and run cmul3_list
    t0 = clock();
    z = cmul4_list(&nums, len - 1);
    t = clock() - t0;

    std::cout << "cmul3_list time (n-1): " << ((double(t) / CLOCKS_PER_SEC)) << std::endl;
    //std::cout << "result: ";
    //z.print();
}

//cmul4
//takes in two complex numbers and then sets z to the product using 4 multiply (n - 1)
void cmul4(myComplex a, myComplex b, myComplex *z) {
    (*z).real.assign((a.real * b.real) - (a.imag * b.imag));
    (*z).imag.assign((a.real * b.imag) + (a.imag * b.real));
}

//cmul3
//takes in two complex numbers and then sets z to the product using 3 multiply (n - 1)
void cmul3(myComplex a, myComplex b, myComplex *z) {
    cpp_int temp1, temp2;
    temp1.assign((a.real * b.real));
    temp2.assign((a.imag * b.imag));

    (*z).real.assign(temp1 - temp2);
    (*z).imag.assign(((a.real + a.imag) * (b.real + b.imag) - temp1 - temp2));
}

//cmul4_list
//takes a vector of complex numbers and recursively multiplies them using 4 multiply (divide and conquer)
myComplex cmul4_list(std::vector<myComplex> *items, int p, int r) {
    if(p < r) {
        int q = (p + r) / 2;
        myComplex a = cmul4_list(items, p, q);
        myComplex b = cmul4_list(items, q + 1, r);
        myComplex c = myComplex();

        c.real.assign((a.real * b.real) - (a.imag * b.imag));
        c.imag.assign((a.real * b.imag) + (a.imag * b.real));

        return c;
    }
    else {
        return (*items)[p];
    }
}

//cmul3_list
//takes a vector of complex numbers and recursively multiplies them using 3 multiply (divide and conquer)
myComplex cmul3_list(std::vector<myComplex> *items, int p, int r) {
    if(p < r) {
        int q = (p + r) / 2;
        myComplex a = cmul3_list(items, p, q);
        myComplex b = cmul3_list(items, q + 1, r);
        myComplex c = myComplex();

        cpp_int temp1, temp2;

        temp1.assign((a.real * b.real));
        temp2.assign((a.imag * b.imag));


        c.real.assign(temp1 - temp2);
        c.imag.assign(((a.real + a.imag) * (b.real + b.imag) - temp1 - temp2));

        return c;
    }
    else {
        return (*items)[p];
    }
}

//cmul4_list
//takes a vector of complex numbers and recursively multiplies them using 4 multiply (n - 1)
myComplex cmul4_list(std::vector<myComplex> *items, int i) {
    //base case
    if(i == 0) {
        return (*items)[i];
    }
    else {
        myComplex a = (*items)[i];
        i--;
        myComplex b = cmul4_list(items, i);
        myComplex c = myComplex();

        c.real.assign((a.real * b.real) - (a.imag * b.imag));
        c.imag.assign((a.real * b.imag) + (a.imag * b.real));

        return c;
    }
}

//cmul3_list
//takes a vector of complex numbers and recursively multiplies them using 3 multiply ( n - 1)
myComplex cmul3_list(std::vector<myComplex> *items, int i) {
    //base case
    if(i == 0) {
        return (*items)[i];
    }
    else {
        myComplex a = (*items)[i];
        i--;
        myComplex b = cmul3_list(items, i);
        myComplex c = myComplex();

        cpp_int temp1, temp2;
        temp1.assign((a.real * b.real));
        temp2.assign((a.imag * b.imag));


        c.real.assign(temp1 - temp2);
        c.imag.assign(((a.real + a.imag) * (b.real + b.imag) - temp1 - temp2));

        return c;
    }
}

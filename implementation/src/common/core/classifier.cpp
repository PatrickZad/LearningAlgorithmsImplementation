#include "classifier.h"
#include "./../../exception/exception.h"
using namespace patrick;

double Classifier::test(double* data, unsigned long width, unsigned long* lable, unsigned long length)
{
    unsigned long error=0;
    for (unsigned long i = 0; i < length; i++)
    {
        if (*(lable+i)!=classify(data+i*width, width))
        {
            error++;
        }
    }
    return 1-(double)error/length;
}
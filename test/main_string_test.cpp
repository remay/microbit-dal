#include "inc/MicroBitTest.h"

#ifdef MAIN_STRING_TEST

#include "MicroBit.h"
#include "ManagedString.h"

MicroBit uBit;
Serial pc(USBTX, USBRX);


void f1(ManagedString s)
{
    pc.printf("f1: %s\n", s.toCharArray());
}

void f2(ManagedString s)
{
    ManagedString s2=s;  
    pc.printf("String: %s\n", s2.toCharArray());
}

void f3(ManagedString s1, ManagedString s2)
{
    ManagedString t = s1+s2;
    
    pc.printf("String: %s\n", t.toCharArray());
}

int main()
{ 
    // Set up debug console.
    pc.baud (115200);

    wait(10.0);
    
    pc.printf("=== Starting String Test ===\n");
    
    ManagedString a("Alice");
    ManagedString b("Bob");
    ManagedString c("Charlie");
    ManagedString d = a.substring(0,3);

    f1(a);
    f2(b);
    f3(c,b);
     
    pc.printf("String: %s\n", d.toCharArray());
    
    while(1)
    {
        wait(1.0);
    }
}

#endif
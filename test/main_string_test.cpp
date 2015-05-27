#include "inc/MicroBitTest.h"

#ifdef MAIN_STRING_TEST

#include "MicroBit.h"

void f1(ManagedString s)
{
#ifdef MICROBIT_DBG    
    pc.printf("f1: %s\n", s.toCharArray());
#endif    
}

void f2(ManagedString s)
{
    ManagedString s2=s;
#ifdef MICROBIT_DBG          
    pc.printf("String: %s\n", s2.toCharArray());
#endif
}

void f3(ManagedString s1, ManagedString s2)
{
    ManagedString t = s1+s2;
    
#ifdef MICROBIT_DBG              
    pc.printf("String: %s\n", t.toCharArray());
#endif    
}

void app_main()
{ 

    
#ifdef MICROBIT_DBG              
    pc.printf("=== Starting String Test ===\n");
#endif    
    
    ManagedString a("Alice");
    ManagedString b("Bob");
    ManagedString c("Charlie");
    ManagedString d = a.substring(0,3);

    f1(a);
    f2(b);
    f3(c,b);
     
#ifdef MICROBIT_DBG              
    pc.printf("String: %s\n", d.toCharArray());
#endif    
    
    while(1)
        uBit.sleep(1000);
}

#endif
/**
  * Class definition for a ManagedString.
  *
  * Uses basic reference counting to implement a copy-assignable, immutable string.
  * This maps closely to the constructs found in many high level application languages,
  * such as Touch Develop.
  *
  * Written from first principles here, for several reasons:
  * 1) std::shared_ptr is not yet availiable on the ARMCC compiler
  * 2) to reduce memory footprint - we don't need many of the other features in the std library
  * 3) it makes an interestin case study for anyone interested in seeing how it works!
  */
  
#ifndef MANAGED_STRING_H
#define MANAGED_STRING_H

#include "MicroBitCompat.h"

class ManagedString
{
    // Internally we record the string as a char *, but control access to this to proide immutability
    // and reference counting.
    char *data;
    int *ref;
    int len;

    public:

    /**
      * Constructor. 
      * Create a managed string from a pointer to an 8-bit character buffer.
      * The buffer is copied to ensure sage memory management (the supplied
      * character buffer may be decalred on the stack for instance).
      *
      * @param str The character array on which to base the new ManagedString.
      */    
    ManagedString(const char *str);
    
    /**
      * Copy constructor. 
      * Makes a new ManagedString identical to the one supplied. 
      * Shares the character buffer and reference count with the supplied ManagedString.
      *
      * @param s The ManagedString to copy.
      */
    ManagedString(const ManagedString &s);
    
    /**
      * Default constructor. 
      *
      * Create an empty ManagedString. 
      */
    ManagedString();
    
    /**
      * Destructor. 
      *
      * Free this ManagedString, and decrement the reference count to the
      * internal character buffer. If we're holding the last reference,
      * also free the character buffer and reference counter.
      */
    ~ManagedString();
    
    /**
      * Copy assign operation. 
      *
      * Called when one ManagedString is assigned the value of another.
      * If the ManagedString being assigned is already refering to a character buffer,
      * decrement the reference count and free up the buffer as necessary.
      * Then, update our character buffer to refer to that of the supplied ManagedString,
      * and increase its reference count.
      *
      * @param s The ManagedString to copy.
      */
    ManagedString& operator = (const ManagedString& s);
    
    /**
      * Equality operation.
      *
      * Called when one ManagedString is tested to be equal to another using the '==' operator.
      *
      * @param s The ManagedString to test ourselves against.
      * @return true if this ManagedString is identical to the one supplied, false otherwise.
      */
    bool operator== (const ManagedString& s);

    /**
      * Inequality operation.
      *
      * Called when one ManagedString is tested to be less than another using the '<' operator.
      *
      * @param s The ManagedString to test ourselves against.
      * @return true if this ManagedString is alphabetically less than to the one supplied, false otherwise.
      */
    bool operator< (const ManagedString& s);

    /**
      * Inequality operation.
      *
      * Called when one ManagedString is tested to be greater than another using the '>' operator.
      *
      * @param s The ManagedString to test ourselves against.
      * @return true if this ManagedString is alphabetically greater than to the one supplied, false otherwise.
      */
    bool operator> (const ManagedString& s);

    /**
      * Extracts a ManagedString from this string, at the position provided.
      *
      * @param start The index of the first character to extract, indexed from zero.
      * @param length The number of characters to extract from the start position
      * @return a ManagedString representing the requested substring.
      */    
    ManagedString& substring(int start, int length);
    
    /**
      * Concatenates this string with the one provided.
      *
      * @param s The ManagedString to concatenate.
      * @return a new ManagedString representing the joined strings.
      */    
    ManagedString& operator+ (ManagedString& s);

    /**
      * Provides a character value at a given position in the string, indexed from zero.
      *
      * @param index The position of the character to return.
      * @return the character at posisiton index, zero if index is invalid.
      */    
    char charAt(int index);


    /**
      * Provides an immutable 8 bit wide haracter buffer representing this string.
      *
      * @return a pointer to the charcter buffer.
      */    
    const char *toCharArray();
    
    /**
      * Determines the length of this ManagedString in characters.
      *
      * @return the length of the string in characters.
      */ 
    int length();

    /**
      * Empty String constant
      */    
    static ManagedString EmptyString;

};

#endif

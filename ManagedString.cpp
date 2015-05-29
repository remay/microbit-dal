#include <string.h>
#include <stdlib.h>
#include "MicroBitCompat.h"
#include "inc/ManagedString.h"

/**
  * Constructor. 
  * Create a managed string from a pointer to an 8-bit character buffer.
  * The buffer is copied to ensure sage memory management (the supplied
  * character buffer may be decalred on the stack for instance).
  *
  * @param str The character array on which to base the new ManagedString.
  */    
ManagedString::ManagedString(const char *str)
{
    len = strlen(str);
    data = (char *) malloc(len+1);
    memcpy(data, str, len+1);
    ref = (int *) malloc(sizeof(int));
    *ref = 1;
}

/**
  * Copy constructor. 
  * Makes a new ManagedString identical to the one supplied. 
  * Shares the character buffer and reference count with the supplied ManagedString.
  *
  * @param s The ManagedString to copy.
  */

ManagedString::ManagedString(const ManagedString &s)
{
    data = s.data;
    ref = s.ref;
    len = s.len;

    (*ref)++;

    if (*ref == 1)
    {
        (*ref)++;
        delete &s;
    }
}


/**
  * Default constructor. 
  *
  * Create an empty ManagedString. 
  */
ManagedString::ManagedString()
{
    data = NULL;
    ref = NULL;
    len = 0;
}

/**
  * Destructor. 
  *
  * Free this ManagedString, and decrement the reference count to the
  * internal character buffer. If we're holding the last reference,
  * also free the character buffer and reference counter.
  */
ManagedString::~ManagedString()
{
    if(data != NULL && --(*ref) == 0)
    {
        delete data;
        delete ref;
    }
}

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

  
ManagedString& ManagedString::operator = (const ManagedString& s)
{
    if(this == &s)
        return *this;
 
    if (data != NULL) 
    {
        if(--(*ref) == 0)
        {
            delete data;
            delete ref;
        }
    }
    
    data = s.data;
    ref = s.ref;
    len = s.len;
    (*ref)++;

    if (*ref == 1)
    {
        (*ref)++;
        delete &s;
    }

    return *this;
}

/**
  * Equality operation.
  *
  * Called when one ManagedString is tested to be equal to another using the '==' operator.
  *
  * @param s The ManagedString to test ourselves against.
  * @return true if this ManagedString is identical to the one supplied, false otherwise.
  */
bool ManagedString::operator== (const ManagedString& s)
{
    if(data == NULL || s.data == NULL)
        return false;

    return (memcmp(data, s.data,min(len,s.len))==0);    
}

/**
  * Inequality operation.
  *
  * Called when one ManagedString is tested to be less than another using the '<' operator.
  *
  * @param s The ManagedString to test ourselves against.
  * @return true if this ManagedString is alphabetically less than to the one supplied, false otherwise.
  */
bool ManagedString::operator< (const ManagedString& s)
{
    if(data == NULL || s.data == NULL)
        return false;

    return (memcmp(data, s.data,min(len,s.len))<0);
}

/**
  * Inequality operation.
  *
  * Called when one ManagedString is tested to be greater than another using the '>' operator.
  *
  * @param s The ManagedString to test ourselves against.
  * @return true if this ManagedString is alphabetically greater than to the one supplied, false otherwise.
  */
bool ManagedString::operator> (const ManagedString& s)
{
    if(data == NULL || s.data == NULL)
        return false;

    return (memcmp(data, s.data,min(len,s.len))>0); 
}

/**
  * Extracts a ManagedString from this string, at the position provided.
  *
  * @param start The index of the first character to extract, indexed from zero.
  * @param length The number of characters to extract from the start position
  * @return a ManagedString representing the requested substring.
  */    
ManagedString& ManagedString::substring(int start, int length)
{
    ManagedString *s;

    if (data == NULL || start >= len)
    {
        s = new ManagedString("");
        *(s->ref) = 0;
        return *s;
    }

    s = new ManagedString();
    length = min(len-start, length);
    s->data = (char *) malloc(length+1);
    memcpy(s->data, &data[start], length);
    s->data[length] = 0;
    s->ref = (int *) malloc(sizeof(int));
    *(s->ref) = 0;

    return *s;
}

/**
  * Concatenates this string with the one provided.
  *
  * @param s The ManagedString to concatenate.
  * @return a new ManagedString representing the joined strings.
  */    
ManagedString& ManagedString::operator+ (ManagedString& s)
{
    if(s.data == NULL)
        return *this;

    if (data == NULL && s.data != NULL)
        return s;

    char *buf = (char *) malloc(len+s.len+1);
    memcpy(buf, data, len);
    memcpy(buf+len, s.data, s.len);
    buf[len+s.len] = 0;

    ManagedString *r = new ManagedString (buf);
    *(r->ref) = 0;
    free(buf);

    return *r;
}


/**
  * Provides a character value at a given position in the string, indexed from zero.
  *
  * @param index The position of the character to return.
  * @return the character at posisiton index, zero if index is invalid.
  */    
char ManagedString::charAt(int index)
{
    return (index >=0 && index < len) ? data[index] : 0;
}

/**
  * Provides an immutable 8 bit wide haracter buffer representing this string.
  *
  * @return a pointer to the charcter buffer.
  */    
const char *ManagedString::toCharArray()
{
    return data;
}

/**
  * Determines the length of this ManagedString in characters.
  *
  * @return the length of the string in characters.
  */ 
int ManagedString::length()
{
    return len;
}

/**
  * Empty string constant literal
  */
ManagedString ManagedString::EmptyString("");



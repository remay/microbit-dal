/**
* Class definition for a Generic Managed Type.
*
* Represents a ref counted type passed through by TD.
*/

#ifndef MICROBIT_MANAGED_TYPE
#define MICROBIT_MANAGED_TYPE
template <class T>
class ManagedType
{
private:

    int *ref;

public:

    T *object;

    /**
    * Constructor for the managed type, given a class space.
    */
    ManagedType(T* object);
    
    /**
    * Default constructor for the managed type, given a class space.
    */
    ManagedType();

    /**
    * Copy constructor for the managed type, given a class space.
    */
    ManagedType(const ManagedType<T> &t);

    /**
    * Destructor for the managed type, given a class space.
    */
    ~ManagedType();

    /**
    * Copy-assign member function for the managed type, given a class space.
    */
    ManagedType<T>& operator = (const ManagedType<T>&i);
    
    /**
    * Returns the references to this ManagedType
    */
    int getReferences();

};

/**
* Constructor for the managed type, given a class space.
*/
template<typename T>
ManagedType<T>::ManagedType(T* object)
{
    this->object = object;
    ref = (int *)malloc(sizeof(int));
    *ref = 1;
}

/**
* Default constructor for the managed type, given a class space.
*/
template<typename T>
ManagedType<T>::ManagedType()
{
    this->object = NULL;
    ref = (int *)malloc(sizeof(int));
    *ref = 0;
}

/**
* Copy constructor for the managed type, given a class space.
*/
template<typename T>
ManagedType<T>::ManagedType(const ManagedType<T> &t)
{
    this->object = t.object;
    this->ref = t.ref;
    (*ref)++;
}

/**
* Destructor for the managed type, given a class space.
*/
template<typename T>
ManagedType<T>::~ManagedType()
{
    if (--(*ref) == 0)
    {
        delete object;
        free(ref);
    }
}

/**
* Copy-assign member function for the managed type, given a class space.
*/
template<typename T>
ManagedType<T>& ManagedType<T>::operator = (const ManagedType<T>&t)
{
    if (this == &t)
        return *this;

    if (--(*ref) == 0)
    {
        delete object;
        free(ref);
    }

    object = t.object;
    ref = t.ref;

    (*ref)++;

    return *this;
}

/**
* Returns the references to this ManagedType
*/
template<typename T>
int ManagedType<T>::getReferences()
{
    return (*ref);   
}
#endif

# LuaCpp2 manual

## Functions
Functions are the main mechanism for abstracting statements and expressions in Lua. It is possible to call a function defined in Lua from C++, as well as to extend Lua with functions defined in C++.

Defining a global function in Lua:
```lua
function add(a, b)
    return a + b
end
```
and the same function in C++:
```cpp
lua.global("add").in() << Lua::MakeFunc(
    [](Lua::Args args, Lua::Lua lua)
    {
        int a, b;
        args.in() >> a >> b;
        args.out() << a + b;
    }
);
```
when calling such a function from C++, there is no difference between functions defined in Lua and functions defined in C++.
```cpp
int result;
lua.pcall("add", 20, 30) >> result;
std::cout << '=' << result << '\n'; // -->  =50
```
### Multiple results
An unconventional, but quite convenient feature of Lua is that functions can return multiple results. We can return as many result as needed by listing them all after the **return** keyword.
```lua
function add(a, b)
    -- return result along with the input arguments
    return a + b, a, b
end

local a, b, result
result, a, b = add(20, 30)
print(a .. " + " .. b .. " = " .. result) -->   20 + 30 = 50 
```
Function defined in C++ can also return multiple results.
```cpp
lua.global("add").in() << Lua::MakeFunc(
    [](Lua::Args args, Lua::Lua lua)
    {
        int a, b;
        args.in() >> a >> b;
        args.out() << a + b << a << b;
    }
);

int result, a, b;
lua.pcall("add", 20, 30) >> result >> a >> b;
std::cout << a << " + " << b << " = " << result << '\n'; // -->  20 + 30 = 50
```
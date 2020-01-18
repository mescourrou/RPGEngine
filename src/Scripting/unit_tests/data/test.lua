function luafunc(e)
    return e:f()
end

function p()
    print("Hello World")
end

print("Hello from the script scope")
a:f()
b:getVal()

function luafunc(e)
    return e:f()
end

function p()
    print("Hello World")
end

print("Hello from the script scope")
b:setValue(10)
if b:getVal() ~= 10 then
    return false
end
return true

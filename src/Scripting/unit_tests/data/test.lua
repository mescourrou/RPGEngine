print("Hello from the script scope")
b:setValue(10)
print(b.m_val)
if b:getVal() ~= 10 then
    return false
end
return true

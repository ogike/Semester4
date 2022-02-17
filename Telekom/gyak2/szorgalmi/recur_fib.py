def fib(num):
    if(num == 1):
        return 1
    elif(num == 0):
        return 0
    else:
        return fib(num-1) + fib(num-2)

print("Hanyadik Fibonacci szám:")
num = int(input())
print(fib(num))
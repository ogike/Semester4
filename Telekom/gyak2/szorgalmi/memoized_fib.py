
def memoize(f):
    memo = {}
    def helper(x):              #we need this wrapper to save the results (the references to memo and f)
        if x not in memo:            
            memo[x] = f(x)
        return memo[x]
    return helper

@memoize        #fib(10)  ->    memoize(fib(10))
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
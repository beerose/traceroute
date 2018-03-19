# Traceroute

Program prints route to target ip.
For TTL from 1 to 30 it sends three packets and waits at most one second for responses. If gets three valid respones the message contains received packets' ip and average response time. In case of one or two valid respones it prints ip and *???* as response time. When no packet comes back program prints `*`.

## Compilation
``` 
    make
```

## Usage

``` 
    sudo ./traceroute address_ip
```

### Example

``` 
    sudo ./traceroute 8.8.8.8
```
### Sample response
```
Trace to: 156.17.254.113
1.      192.168.137.1   ???
2.      *
3.      10.10.66.1      28ms
4.      188.121.30.142  14ms
5.      216.66.87.145   36ms
6.      184.105.65.73   29ms
7.      193.203.0.172   40ms
8.      62.40.98.48     33ms
9.      83.97.88.122    69ms
10.     212.191.224.106 39ms
11.     156.17.254.113  34ms
Done!
```
## Clean
```
    make clean
```
## Clean all
```
    make distclean
```
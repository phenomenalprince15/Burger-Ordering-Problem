# Requirements

## Introduction
* Use of IPCs, threads and semaphores in understanding of OS.

# Problem Description
1. In a burger shop, there are cooks, cashiers and customers.
2. There is a rack to place burgers.
3. When a customer will order, if cashier is awake, he can check the rack if burger's available, if not, he can ask cook and wait for sometime. If cashier is not awake, customer will wait.
4. So, we can create 3 threads for each one of those processes, one for each cooks, cashiers and customers.
5. Using signalling mechanism by semaphores, we can send signals between these threads for interactions.

## Cost and Features
### *Cost*
Since the algorithm can be executed on any software having gcc installed, it's free of cost.

## Defining the System
![System-Diagram](https://github.com/phenomenalprince15/Burger_Ordering_Problem/blob/main/1_Requirements/system.jpg)

## SWOT Analysis
![SWOT-Analysis](https://github.com/phenomenalprince15/Burger_Ordering_Problem/blob/main/1_Requirements/swot_analysis_burger.jpg)

# 4W&#39;s and 1&#39;H
## WHO
This is helpful in understanding the threads and IPCs concept in operating system.

## WHAT
It gives you output for mechanism between customers, cashiers and cooks.

## WHEN
When there is an interaction between customers and owners like in restaurants.

## WHERE
It can be put to use in where there is an interaction between atleast 3 persons.

## HOW
The system used is visual studio code for implementation in C programming language, just go the implemenation folder and run command "make target" to build and run using "make run". Give your own inputs.

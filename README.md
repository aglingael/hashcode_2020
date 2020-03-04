This repository contains my implementation for google hashcode 2020

May be the files' path won't be correct on your machine. Pay attention to that.

I implemented two solutions:

* This first one is simple greedy function to define the order of signup.
The result is then straightforward from this order. It reached about 22M points.
* The second is a local search implementation working like CART algorithm for 
decision trees. It reached about 26.8M points. It can be improved in two points:
    * Currently only intensification is implemented so the algorithm is like
     hill-climbing and reachs a local optimum. A diversifcation approach could escape 
     the local optimum and reach a good result. I provide some tips in the code on
     how to add it.
     * The second point of improvement could be function used to compare two libraries.
     My function in the file class.cpp (line 244). May be it can be improved.
     I'm not sure :-)
     * May be the combination of the two previous implemented can also help.
     You can try ;-)
 
 Try all you want and have fun (o.~) 
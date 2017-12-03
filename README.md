# validate_social_learning
The code and results for the paper https://dl.acm.org/citation.cfm?doid=3067695.3082550
Utilizes an extended/modified version of https://github.com/nekonaute/roborobo3
A simulation/replication of the experiment presented in http://www.cs.vu.nl/~gusz/papers/2015-Evolution,%20Individual%20Learning,%20and%20Social%20Learning%20in%20a%20Swarm%20of%20Real%20Robots.pdf

The properties used for the setup in the paper can be found at /src/roborobo/config/sociallearning.properties


# TODO
Change the stupid way in which the evaluators each consumes a thread, 
should only be 1 that evaluates every robot instead of the other way around

# Potential change
Instead of creating an Input node for each sensor, instead use the original setup and then accumulate the sensor input of the additional sensors so it dosen't alter the original design.

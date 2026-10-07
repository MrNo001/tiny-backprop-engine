# Small backpropogation engine

The Value class is the core unit of the engine, constructing a DAG with every operation.

Calling backwards on the one node propogates it's gradients to it's children.

The  Value::backwards method sorts the nodes in topological order and call's backwards in reverse order for the node members of each value. 


The engine is based on the Andrej Karapathy series NN zero to hero.


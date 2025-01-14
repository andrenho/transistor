There are two main modules: **game** and **ui**. They interact to each other via a public interface.

Data in the modules can be _read_ through the const members of the module classes, but can only be modified
by putting requests in the interface queue.
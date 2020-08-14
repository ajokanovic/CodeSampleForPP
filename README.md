# Entity Component System sample

## Introduction

This repository holds the code I wrote as a freelancer for a client from Belgrade. The project requirement was to write an example **entity component system** for a 3D engine written in C  that the client was working on. This code is just a sample of what I added, simplified for 2D. It is not complete and it cannot be run on its own.

I chose this project as it was the most recent thing I have written between working at LotusFlare and Reves Technologies. In both companies I have worked on projects that I do not feel comfortable sharing the source from because of my agreements with them.

# What is ECS?
[ESC]("https://en.wikipedia.org/wiki/Entity_component_system")  stands for **entity component system** , it is an architectual pattern that is mostly used in game development. ECS follows the [composition over inheritance](https://en.wikipedia.org/wiki/Composition_over_inheritance "Composition over inheritance") principle that allows greater flexibility. It also goes hand in hand with the  [data oriented design](https://en.wikipedia.org/wiki/Data-oriented_design). As a result has become the most popular pattern used by todays [game engines](https://en.wikipedia.org/wiki/Game_engine).

Within this project, as you will see, most of the data is organized to allow the data oriented programming approach, but it is not fully there yet. More on that in the **Improvements** section.

The core idea of this design is to divide the code into 3 main categories:
 - Components
 - Entities
 - Systems

## Components
Components in short represent - **data**. They are data structures holding the data that describes a certain behavior or property of an object in the game world. You can kind of look at them as the attributes inside a class in OOP.

### Example:
Let’s for example imagine that we want to create a tree that can grow as the time passes. We would want to specify the growth period, a visual scaling factor, max growth, etc. These parameters would be grouped under one component, for example, named **GrowthComponent**.

This tree would also need its position, rotation and scale in the world. These 3 parameters are known as a transformation, so if we group them together we can name them a **TransformComponent**

## Entities
If you look at the real world you can see that with enough components you could describe every physical thing as a collection of components and entities are just that.

One entity can have one or more components describing that entity.

### Example:

In the tree example from above, the tree itself would be the entity, made out of all the mentioned components. In other words the tree would be represented as a list of these components:
	

    tree = [GrowthComponent, TransformComponent]

In another example lets say we want to experiment and decide to assign the **GrowthComponent** to a simple rock. This **rock** would then grow the same way the **tree** would.

## Systems
We mentioned that components are just collections of data describing certain properties of an entity. And entities are just collections of those components. One thing that is missing is the logic that does all the processing. That’s where the systems come in. Systems are functions that take certain types of components and execute some logic for all of them.

I consider systems as having 3 main types:

- One component systems – they just go through the list of components of a certain type and do something for all of them

- Entity systems – they go through lists of components of multiple types, but only do processing on those that are a part of the same entity

- General systems – they go through lists of components, executing the code for every combination of components from those lists.

### Example:
An example of a **one component system** would be the system that goes through all **GrowthComponents** and calculates how much that component has “grown”.

An example of an **entity system** would be a system that takes the **GrowthComponent** and a **TransformComponent** that must be a part of the same entity and then applies the scale change from the growth component to the **TransformComponent** scale.

An example of a **general system** would be the system that for each component of type **A** goes through the whole list of the component **B**. These were cut from the requirement and were never implemented in this project.


![enter description here](https://docs.unity3d.com/Packages/com.unity.entities@0.9/manual/images/ECSBlockDiagram.png)
# Project structure

This project, like most of the C project, is divided into two parts. The **headers**, holding just the function definitions, constants, includes between modules and macros. And the **sources**, where everything from the headers is implemented. Specific files & directories are described below.

## Ecs 
The **ecs** directory holds the containers holding all the necessary information about components, entities and systems. Inside these files all the ecs magic happens.

## Concrete
The **concrete** directory holds concrete definitions of entities, components and systems.

All the components are defined in the file **components.h** located in the **concrete** directory inside the **headers**, and the code that calculates their individual size is in the components.c located in the **concrete** directory inside the **sources**.

Entitys are created by initializing the components first and then grouping them into an entity. This is done for each entity type and the functions that do that are inside the **entities.h** header file, while their implementations are in the source file **entities.c**.

Lastly the systems are defined inside the **systems.h** file following the same file structure pattern as both the entities and components follow. Systems implementations are inside the **systems.c** source file.

## Utils
The **utils** directory holds some utility things such as:

**Vector** (dinamyc array) implementations for integer and float values, as well as a “generic” (pointer) vector implementation. These are designed to allow adding new vector types in a matter of minutes.

**Debug functionalities** that are used to replace memory management functions such as **malloc** & **free**. Their purpose is to track memory usage, locate and log any memory leaks. These are turned on by defining a specific macro, as is a common thing to do in C projects.

## Axiom
As the engine I was sent was called “Axiom” there are **axiom.h** and **axiom.c** files that bring all the functionalities together. Here the engine **initialization**, **inner loop** and **cleanup** are defined.

The engines **initialization** routine consists of: loading all resources, initializing all the ecs containers, registering all of the systems and dividing them into system groups so they can be updated separately.

**The inner loop** is what keeps track of time and updates system groups accordingly.

**Cleanup** is self-explanatory, cleaning all the used memory and releasing resources.

## Main
**main.c** is the entrypoint of the application, when run it starts the engine.

# Implementation
Here we will discuss the implementation in a bit more detail.
## Containers

As mentioned before containers are used to manage storage used by the entities, components and systems. They all function similarly, but also have their own quirks.

It is quite common for structures such as vectors, to which containers are similar, to possess:

- **data** -> Data refers to the memory buffer that holds the raw data
- 
- **capacity** -> refers to the amount of memory that is available in the data buffer or in other words it is the total size of the data buffer.
- 
- **size** -> is the amount of that memory that is occupied by the real data. The name “size” for this parameter comes from the C++ [vector::size()](“http://www.cplusplus.com/reference/vector/vector/size/”)

### Components container
Before we can understand how component containers work we need to see how a simple component looks and works.
#### Components
Lets take the TransformComponent straight out of the code:

    typedef struct
    {
    	ComponentHeader header;
    	vec3  		position;
    	vec2  		scale;
    	float  		rotation;
    
    }TransformComponent;

The first thing to notice is that there is a **ComponentHeader** in every component, and it is always the first member of the structure. Here is the definition of the  **ComponentHeader** structure as well as the **ComponentType** enum:

    typedef enum
    {
    	CMP_TRANSFORM,
    	CMP_STATIC_SPRITE,
    	CMP_DYNAMIC_SPRITE,
    	CMP_MULTY_DYNAMIC_SPRITE,
    	CMP_RIGID_BODY,
    	CMP_PLAYER,
    	CMP_CNT
    
    }ComponentType;
    
    typedef struct
    {
    	ComponentType type;
    	void* entity;
    
    }ComponentHeader;

So there are two very neat things happening here. First the **ComponentType** has **CMP_CNT** as the last member, and as *enum members are represented as sequential numbers*, **CMP_CNT** will always hold the total number of component types defined.

Now we see that **ComponentHeader** holds the **type of the component** and also holds a reference to an **entity** it is a part of.

> Note: This is a bit of a sloppy solution and can be extended so that
> the component can be a part of multiple entities, but having
> non-sharable components suited the client’s needs at the time so it
> was left like this.

Now if we look back to what is common for all components, we can see that it is the **header**. When this header is the first member of the structure that allows us to **cast** any of the pointers to components into a **ComponentHeader** pointer:

    GrowthComponent growthComponent;
    ComponentHeader* component = (ComponentHeader*) &growthComponent;

It orks because pointers point to the address, and the first member will always align with that address.
This can then be used to generalize the logic surrounding components, making them a bit easier to manage.

You can maybe look at this behavior as some form of *structure inheritance in C.*

> Note: this only works if the structure member order is guaranteed.

![enter image description here](https://slideplayer.com/slide/8678982/26/images/13/Layout+of+Structures+in+Memory%281%29.jpg=400x300)
#### Back to containers
Here is the definition of the component container structure:

    typedef struct 
    {
    	ComponentHeader* components[CMP_CNT];
    	uint sizes[CMP_CNT];
    	uint capacities[CMP_CNT];
    	char resort_systems[CMP_CNT];
    	char resort_sys_flag;
    	
    }ComponentContainer;

Here you can clearly see the application of having the **header** as the first member of the component structures. We can now create a table of all components just by using **ComponentHeaders**, this could also be done with **void** **pointers** but this gives the ability to **get the type** of the component before casting.

The component container behaves like an **array of vectors** so it has all the things that the vector would have but in plural:
 - **components** - this is a table of components. Every "row" is a vector that holds all the components of certain type.  
 - **capacities** - array of capacities for each of the aforementioned vectors.
 - **sizes** - array of current sizes for each of the aforementioned vectors.
 - **resort_systems** - array of booleans that specifies if a component lists of all systems using this component should be resorted. This happens when any changes in the number of components happen and it is useful because it decreases the number of components the system has to go trough to find components belonging to the same entity.
 - **resort_sys_flag** - a regular boolean that is set if any changes happened to the number of components.
 
 
![enter image description here](https://i.ibb.co/dbNQ5G6/Component-container.png)

Functions for working with this container are:
- **component_container_init** - initializes the members and clears the initial data buffer
- **component_container_cleanup** - frees all the memory it used
- **component_container_allocate** - creates a new component of a desired type and returns a *ComponentHeader* pointer to it.
- **component_container_remove** - removes the component from the container, also removes it from the entity tied to it.
- **component_container_get** - retrieves a component of certain type in a certain position from the container.

Aside from some akward casting needed because of the generic nature of the container, there is nothing special here compared to the regular vector implementation. 


### Entities container
Entity container is basically a vector with additional logic, but most of that logic was removed for this sample presentation.

As entity containers work with entities, here is their definition:

    typedef struct
    {
    	uint number_of_components;
    	ComponentHeader** components;
    
    }Entity;
    
As you can see entities just have a **number of components** and a **list of references** to components that describe it. 

> Note: This is in my opinion one of the places where great improvements
> can be made as this is not *"data orientation friendly"* because we do
> not use entities as real storage for the components, we use references
> to them.

Functions for working with this container are:
- **entity_container_init** - initializes the members and clears the initial data buffer
- **entity_container_cleanup** - frees all the memory it used (does not free components!)
- **entity_container_allocate** - creates a new entity and returns a reference to it.
- **entity_container_remove** - removes the entity from the container, also calls **component_container_remove** on all components inside it.
- **entity_find_component** - used to fetch a certain component by type from the entity. Returns a **ComponentHeader** reference.


### Systems container
System container is actually *an array of system containers* because I wanted to have the ability to split systems into **groups** that would be run at *different times*. For example, **render systems** and **logic systems** would be run at different pace than **physics systems**.

Here is the definition of all supported types:

    typedef enum
    {
   	    SYS_MAIN,
   	    SYS_RENDER,
   	    SYS_INTERACT,
   	    SYS_TYPE_CNT
   	    
    }SystemType;

Adding a new type above **SYS_TYPE_COUNT** will allow future use of that type.

The systems themselves just have a vector of **component types** they are interested in, and an **update function** that is called when the system is updated:

    typedef struct
    {
	    Vector_ui components; // types
	    SYSTEM_FUNCT_PTR_TYPE(update);
    }System;

You can notice that systems use some obscure **function pointers**, those can be too long to remember, and are used to define all update functions, so I decided to hide them under some macros:

    #define SYSTEM_FUNCT_PTR_TYPE(name) void(*name)(double delta, uint number_of_components, ComponentHeader** components)
    #define SYSTEM_FUNCT_TYPE(name) void name (double delta, uint number_of_components, ComponentHeader** components)
    #define SYSTEM_EXTRACT_COMPONENT(type) system_extract_component(number_of_components, components, type)

Whenever an **update system function** is defined, `SYSTEM_FUNCT_TYPE(function_name)` should be used as its header (signature).

Functions for working with this container are:
- **systems_container_init** - initializes all the vectors and the space needed.
- **systems_container_cleanup** - cleans up all the memory that the systems used.
- **system_container_allocate** - creates a new system of a given type, update function, and a variable list of components it will update with. This new system is inserted into the proper container and a reference to it is returned.
- **systems_sort_components** - sorts systems components vector so that the components with fewest instances go first. This is so that the system has a smaller set of components it has to go through to find all required components that share the same entity.
- **system_extract_component** - used from within the system update function to get the components, as components coming in as arguments to the update function don't have the same order all the time.
- **update_all_systems** - this is the function that updates all the system. It is where the magic happens.

### Updating all systems

When systems of a certain type are updated it is checked for each of them if they are a "**one component system**" or an "**entity system**”. If it is a **one component system**, it does the processing by just going through the related row in the component container running the update function for each component.

If the **system requires multiple components from an entity**, it will first check if it needs to resort its component vector. After potential resorting it will extract the component from the component container and from that component get the **entity**. This **entity** will then be used to find other components that the system needs. These components are added to an array that is then passed to the systems update function.

In the update functions of a multy component system, components are retrieved by using a macro `SYSTEM_EXTRACT_COMPONENT(CMP_TYPE)` where **CMP_TYPE** is the type of the component to retrieve.

![enter image description here](https://i.ibb.co/SPCzRNJ/diagram.png)

## Example usage
This is by no means written in stone, the structure can be changed and broken down even more. Example usage of the described system would go something like this:

**1.** Add desired components & their type signature into the **headers\concrete\components.h** file following the convention:
 

     typedef struct
    {
    	ComponentHeader header;
    	float  			growth_rate;
    	float			max_growth;
    
    }GrowthComponent;
     
    typedef enum
    {
    	CMP_TRANSFORM,
    	CMP_STATIC_SPRITE,
    	CMP_DYNAMIC_SPRITE,
    	CMP_MULTY_DYNAMIC_SPRITE,
    	CMP_RIGID_BODY,
    	CMP_PLAYER,
    	CMP_GROWTH, <----
    	CMP_CNT
    
    }ComponentType;

**2.** Append their size to the cases in the **sources\concrete\components.c**:


    uint component_get_size(ComponentType type)
    {
    	switch(type)
    	{
    		case CMP_TRANSFORM:
    			return sizeof(TransformComponent);
    		break;
    
    		case CMP_STATIC_SPRITE:
    			return sizeof(StaticSpriteComponent);
    		break;
    
    		case CMP_GROWTH:
    			return sizeof(GrowthComponent);
    		break;
    	}
    }

 **3.** Create an entity that uses that component. This can be done anywhere, I will just add another function to the  **sources\concrete\entities.c**:

     void entities_add_growing_object(float growth_rate, float growth_max, TransformParams transform)
    {
    	GrowthComponent*    growth_comp    = component_container_allocate(CMP_GROWTH);
    	TransformComponent* transform_comp = component_container_allocate(CMP_TRANSFORM);
    	entity_container_allocate(2, growth_comp, transform_comp);
    	
    	growth_comp->growth_rate  = growth_rate;
    	growth_comp->max_growth   = growth_max;
    									
    	V3_FILL(transform_comp->position, transform.x, transform.y, transform.z);
    	V2_FILL(transform_comp->scale, transform.sx, transform.sy);
    	transform_comp->rotation = transform.rot;
    }

 **4.** Create a new update system function signature in  **headers\concrete\systems.h**:

     SYSTEM_FUNCT_TYPE(system_growth);
     
**5.** Create an implementation for this system update function in  **sources\concrete\systems.c**:    
 

    SYSTEM_FUNCT_TYPE(system_growth)
    {
        GrowthComponent* growth = SYSTEM_EXTRACT_COMPONENT(CMP_GROWTH);
        TransformComponent* transform = SYSTEM_EXTRACT_COMPONENT(CMP_TRANSFORM);
    
        //DO SOMETHING WITH COMPONENTS
    }
    
**6.** Register the system update function in one of the system groups in the main part of the engine:
  

     AXIOM_ADD_SYSTEM(SYS_MAIN, system_growth, 2, CMP_GROWTH, CMP_TRANSFORM);


This can surely seem like a lot of work, but the toolset that was available was fairly limiting. One of the ideas we brainstormed was to have lua as the scripting language on top of the engine which would encapsulate all the ugly boilerplate work.

# Improvements

Some of the things that I find are missing or problematic are:
- Components cannot be shared by multiple entities
- There are no “general systems”
- There is a question of should the entities be the “storage”, that might help performance as it is cache friendly. But then would it impact the single component systems?
- Components should be created not at compile time but at runtime, this would greatly increase the capabilities of the system.
- Mapping of component types to their sizes is not the happiest solution.

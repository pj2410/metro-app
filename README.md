# Metro App: Finding Routes over a Given Map using Dijkstra's Algorithm
============================================================

## Overview
-----------

This is a simple C++ program that helps commuters navigate the Delhi Metro network. It takes the source station and destination station as input from the user and displays the shortest metro route, fare, and distance to reach the destination.

## Implementation
-----------------

### Nodes

* **Represent a metro station**
* **Contain information about the station, including:**
	+ Name
	+ Metro corridor
	+ Lines connected to the station

### Edges

* **Represent the connection between two nodes (stations)**
* **Have a cost equal to the distance between the two connecting stations**

## Algorithm
-------------

The program uses **Dijkstra's algorithm** to find the shortest path between the source station and the destination station. The algorithm works as follows:

1. **Initialize the distance to the source station as 0 and all other stations as infinity.**
2. **Create a priority queue to store stations to be processed, with the source station as the first node.**
3. **While the queue is not empty, extract the station with the minimum distance and mark it as processed.**
4. **Update the distances of the neighboring stations that have not been processed yet.**
5. **Repeat steps 3-4 until the destination station is reached.**

## Output
---------

The program displays the following information to the user:

* **Shortest metro route between the source station and the destination station**
* **Total fare to travel between the two stations**
* **Total distance between the two stations**

## Features
------------

* **User-friendly interface to input source and destination stations**
* **Displays the shortest metro route, fare, and distance**
* **Uses Dijkstra's algorithm to find the optimal route**
* **Can be easily extended to include more metro stations and lines**

## Future Enhancements
----------------------

* **Integrate with a database to store and update metro station information**
* **Add more algorithms to find the shortest path (e.g., breadth-first search, depth-first search)**
* **Implement a more advanced user interface (e.g., GUI) for better user experience**
* **Add the feature where if a station's activity is disrupted it can give a new path**

# Example of overlapping QGraphicsItem and hover events

This repository contains a basic example of what I am trying to accomplish in QT with respect to resizing an image. I have a basic (but buggy) version of code that adds two QGraphicsItems to a scene. The images can be moved around and can be resized. I'm trying to figure out how to allow the hoverMoveEvent to propagate to the GraphicsItem that is being occluded. 

**Success is:** being able to have the cursor change to a resize cursor when hovering over an occluded corner of an image that is partially occluded by another image. The unoccluded parts of the selected image will show the correct hover behavior when hovering over the other corners.

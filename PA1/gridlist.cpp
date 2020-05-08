// File:        gridlist.cpp
// Date:        2020-01-18 00:11
// Description: Contains partial implementation of GridList class
//              CPSC 221 2019W2 PA1
//              Function bodies to be completed by yourselves
//
// ADD YOUR FUNCTION IMPLEMENTATIONS IN THIS FILE
//

#include "gridlist.h"
#include "gridlist_given.cpp"

// Creates a PNG of appropriate pixel dimensions according to the GridList's structure
//   and colours each pixel according the each GridNode's Block data.
// The fully coloured PNG is returned.
PNG GridList::Render() const {
  PNG image;
  GridNode* temp = northwest;
  int dimension = northwest->data.Dimension();
  image.resize(dimx * dimension, dimy * dimension);
  int x = 0;
  int y = 0;
  if (northwest != NULL) {
    while (temp -> next != NULL) {
      temp->data.Render(image,y* dimension,x * dimension);
      x++;
      if (x == dimx) {
        y++;
        x = 0;
      }
      temp = temp -> next;
    }
    temp->data.Render(image,y* dimension,x * dimension);
  }
  return image;
}

// Allocates a new GridNode containing bdata and
//   attaches it to end of this list.
// Be careful of the special case of inserting into an empty list.
void GridList::InsertBack(const Block& bdata) {
  GridNode* insert = new GridNode(bdata);
  if (northwest == NULL) {
    northwest = insert;
    insert = southeast;
  } else {
    GridNode* temp = northwest;
    while (temp -> next != NULL) {
      temp = temp -> next;
    }
    temp -> next = insert;
    insert -> prev = temp;
    southeast = insert;
 }
}

// if this list has an odd number of column blocks, then the right side will have more blocks
// PRE:  this list and inner are not the same physical list
//       this list must have at least two column blocks
//       inner list must have at least one column block
//       inner list must have the same vertical resolution, vertical block dimension, and block size
// POST: this list has nodes of the entire inner list in between the left and right halves of this list
//         and inner list becomes empty
//       otherwise both lists are not modified
// THIS MUST BE ACHIEVED USING POINTER REASSIGNMENTS.
// DO NOT ALLOCATE OR DELETE ANY NODES IN THIS FUNCTION.
void GridList::Sandwich_H(GridList& inner) {
if (dimy == inner.dimy && inner.Length() != 0 && northwest != NULL) {
  int column = dimx / 2;
  GridNode* curr = northwest;
  int i = 1;
  while (curr->next != NULL) {
    if (i % dimx == column) {
    for (int j=0; j<inner.dimx;j++) {
      GridNode* transfer = inner.northwest;
      GridNode* next = curr->next;
      inner.northwest = inner.northwest->next;
      transfer->prev = curr;
      transfer->next = next;
      curr->next = transfer;
      next->prev = transfer;
      curr = curr -> next;
    }
    }
    i++;
    curr = curr->next;
  }
  dimx += inner.dimx;
}
}

// inner list must have the same horizontal resolution, horizontal block dimension, and block size
// if this list has an odd number of row blocks, then the bottom side will have more blocks
// PRE:  this list and inner are not the same physical list
//       this list must have at least two row blocks
//       inner list must have at least one row block
//       inner list must have same horizontal resolution, horizontal block dimension, and block size
// POST: this list has nodes of the entire inner list in between the upper and lower halves of this list
//         and inner list becomes empty
//       otherwise both lists are not modified
// THIS MUST BE ACHIEVED USING POINTER REASSIGNMENTS.
// DO NOT ALLOCATE OR DELETE ANY NODES IN THIS FUNCTION.
void GridList::Sandwich_V(GridList& inner) {
  if (dimx == inner.dimx && inner.Length() != 0 && northwest != NULL) {
  int row = dimy / 2;
  int beginning = dimx * row;
  int i = 1;
  GridNode* temp = northwest;
  while (temp -> next != NULL) {
    i++;
    temp = temp -> next;
    if (i == beginning) {
      while (inner.northwest->next != NULL) {
        GridNode* oldNext = temp -> next;
        GridNode* transfer = inner.northwest;
        inner.northwest=inner.northwest->next;
        temp->next = transfer;
        transfer->prev = temp;
        transfer->next = oldNext;
        oldNext->prev = transfer;
        temp = temp->next;
      }
      GridNode* oldNext = temp -> next;
      GridNode* transfer = inner.northwest;
      inner.northwest=inner.northwest->next;
      temp->next = transfer;
      transfer->prev = temp;
      transfer->next = oldNext;
      oldNext->prev = transfer;
      dimy += inner.dimy;
      return;
    }
  }
  }
}


// PRE:  this list and otherlist have the same pixel dimensions, block dimensions, and block size
// POST: this list and otherlist are checkered with each other's nodes
//       each list's dimensions remain the same
// THIS MUST BE ACHIEVED USING POINTER REASSIGNMENTS.
// DO NOT ALLOCATE OR DELETE ANY NODES IN THIS FUNCTION.
void GridList::CheckerSwap(GridList& otherlist)
{
  int i = 0;
  int row = 0;
  if (northwest != NULL && dimx == otherlist.dimx && dimy == otherlist.dimy) {
    GridNode* head = northwest;
    GridNode* head2 = otherlist.northwest;
  if (northwest -> next != NULL && otherlist.northwest->next != NULL) {
    for (int i = 1; i <= otherlist.Length(); i++) {
      if (i % 2 == 0) {
        if (head->next != NULL && head2->next != NULL) {
        GridNode* headPrev = head->prev;
        GridNode* head2Prev = head2->prev;
        GridNode* headNext = head->next;
        GridNode* head2Next = head2 -> next;
        headPrev->next = head2;
        headNext->prev = head2;
        head2->prev = headPrev;
        head2->next = headNext;
        head2Prev->next = head;
        head2Next->prev = head;
        head->next = head2Next;
        head->prev = head2Prev;
      } else {
        GridNode* headPrev = head->prev;
        GridNode* head2Prev = head2->prev;
        headPrev->next = head2;
        head2->prev = headPrev;
        head2Prev->next = head;
        head->prev = head2Prev;
      }
      }
      head = head -> next;
      head2 = head2 -> next;
    }
  } 
}
}

// POST: this list has the negative effect applied selectively to GridNodes to form
//         a checkerboard pattern.
//       The northwest block is normal (does not have the negative effect applied).
// Ensure that the checkering looks correct for both odd and even horizontal block dimensions
void GridList::CheckerN() {
  GridNode* head = northwest;
  for (int i = 0; i < (dimy / 2); i++) {
    for (int j = 0; j < dimx; j++) {
      if (j % 2 == 1) {
        head -> data.Negative();
      }
      head = head -> next;
    }
    for (int j = 0; j < dimx; j++) {
      if (j % 2 == 0) {
        head -> data.Negative();
      }
      head = head -> next;
    }
  }
  if (dimy % 2 == 1) {
    for (int j = 0; j < dimx; j++) {
      if (j % 2 == 1) {
        head -> data.Negative();
      }
      head = head -> next;
    }
  }
}

// Deallocates any dynamic memory associated with this list
//   and re-initializes this list to an empty state
void GridList::Clear()
{
  while (northwest != NULL) {
    if (northwest -> next == NULL) {
      northwest = NULL;
    } else {
      GridNode* temp = northwest;
      northwest = northwest -> next;
      temp -> next = NULL;
      northwest -> prev = NULL;
      temp = NULL;
    }
  }
}

// Allocates new nodes into this list as copies of all nodes from otherlist
void GridList::Copy(const GridList& otherlist)
{
  if (otherlist.northwest != NULL) {
    Block& temp = otherlist.northwest->data;
    InsertBack(temp);
    GridNode* node = otherlist.northwest;
    while(node->next != NULL) {
      Block& temp = otherlist.northwest->data;
      node = node->next;
      InsertBack(temp);
    }
  }
}

// IMPLEMENT ANY PRIVATE FUNCTIONS YOU HAVE ADDED BELOW
//
//
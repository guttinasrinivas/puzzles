package main

import (
	"fmt"
	"strings"
)


type WordNode[T any] struct {
	next *WordNode[T]
	prev *WordNode[T]
	parent *WordNode[T]
	data *T
}


type WordQueue[T any] struct {
	head *WordNode[T]
	tail *WordNode[T]
	size int32
}


func (wq *WordQueue[T]) Push(data *T) {
	node := &WordNode[T]{
		next: nil,
		prev: wq.tail,
		parent: nil,
		data: data,
	}

	if wq.tail != nil {
		wq.tail.next = node
	}

	if wq.head == nil {
		wq.head = node
	}

	wq.tail = node
	wq.size++
}


func (wq *WordQueue[T]) Pop() *T {
	if wq.size == 0 {
		return nil
	}

	node := wq.head

	if wq.head.next != nil {
		wq.head = wq.head.next
		wq.head.prev = nil
	} else {
		wq.head = nil
		wq.tail = nil
	}

	wq.size--
	return node.data
}


func (wq *WordQueue[T]) Size() int32 {
	return wq.size
}


func (wq *WordQueue[T]) IsEmpty() bool {
	if (wq.head != nil) ||
	   (wq.tail != nil) ||
	   (wq.size != 0) {
		return false;
	}
	return true;
}


func (wq *WordQueue[T]) String() string {
	var sb strings.Builder
	sb.WriteString("WordQueue: ")
	sb.WriteString(fmt.Sprintf("Size: %d", wq.size))
	sb.WriteString("\n")
	node := wq.head
	sb.WriteString("Contents:\n")

	for node != nil {
		sb.WriteString(fmt.Sprintf("%v\n", node.data))
		node = node.next
	}

	return sb.String()
}


/* End of file */


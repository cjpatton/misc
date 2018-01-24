package main

import (
	"fmt"
	"strings"
)

func isort(words []string) {
	var tmp string
	for i := 0; i < len(words); i++ {
		for j := i; j < len(words); j++ {
			if strings.Compare(words[i], words[j]) == 1 {
				tmp = words[i]
				words[i] = words[j]
				words[j] = tmp
			}
		}
	}
}

func main() {
	//words := []string{"hella", "cool", "stuff"}
	words := make([]string, 4)
	words[0] = string("hella")
	words[1] = string("cool")
	words[2] = string("fun")
	words[3] = string('a')
	isort(words)
	for i := 0; i < len(words); i++ {
		fmt.Println(words[i])
	}
}

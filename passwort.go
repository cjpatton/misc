// Generate a base64 password with 128 bits of entropy and output to stdout.
package main

import (
	"crypto/rand"
	enc "encoding/ascii85"
	"fmt"
	"os"
)

func main() {

	bytes := 16
	key := make([]byte, bytes)
	if n, err := rand.Read(key); err != nil {
		fmt.Fprintln(os.Stderr, "something bad happened:", err)
	} else if n < bytes {
		fmt.Fprintf(os.Stderr, "couldn't read %d bytes for some reason", bytes)
	}

	encoder := enc.NewEncoder(os.Stdout)
	defer encoder.Close()

	encoder.Write(key)
}

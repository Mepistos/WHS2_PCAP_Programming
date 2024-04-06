/**
 * TCPServer.go
 **/

package main

import ("bytes"; "fmt"; "net")

func main() {
    serverPort := "12000"

    listener, _:= net.Listen("tcp", ":" + serverPort)
    fmt.Printf("Server is ready to receive on port %s\n", serverPort)

    buffer := make([]byte, 1024)

    conn, _:= listener.Accept()
    for {
        fmt.Printf("Connection request from %s\n", conn.RemoteAddr().String())
        count, _ := conn.Read(buffer)
	if buffer[0]=='0' {
	        break
	}
        conn.Write(bytes.ToUpper(buffer[:count]))
    }
    conn.Close()
    fmt.Printf("%s Disconnected\n", conn.RemoteAddr().String())
}


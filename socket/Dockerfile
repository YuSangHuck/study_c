# Use an official GCC runtime as a parent image
FROM gcc:latest

# Set the working directory
WORKDIR /usr/src/app

# Copy the current directory contents into the container at /usr/src/app
COPY . .

# Compile the blocking server
RUN gcc -o blocking_server server/blocking_server.c

# Compile the non-blocking server
RUN gcc -o nonblocking_server server/nonblocking_server.c

# Install necessary tools
RUN apt-get update && apt-get install -y netcat-openbsd iproute2

# Define the command to run the servers
CMD ["bash", "-c", "./blocking_server & ./nonblocking_server"]

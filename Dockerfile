FROM ubuntu:24.04

# 1. Install dependencies
# We need build-essential for GCC/OpenMP and OpenMPI for distributed logic
RUN apt-get update && apt-get install -y \
    build-essential \
    openmpi-bin \
    libopenmpi-dev \
    openssh-server \
    openssh-client \
    && apt-get clean \ 
    && rm -rf /var/lib/apt/lists/*

# 2. Setup SSH for MPI communication
# MPI uses SSH to launch processes on remote nodes
RUN mkdir /var/run/sshd
RUN echo 'root:mpi' | chpasswd
RUN sed -i 's/#PermitRootLogin prohibit-password/PermitRootLogin yes/' /etc/ssh/sshd_config

# 3. Generate SSH keys so the Master can login to Workers automatically
RUN ssh-keygen -t rsa -f /root/.ssh/id_rsa -N "" \
    && cp /root/.ssh/id_rsa.pub /root/.ssh/authorized_keys

# 4. Set working directory
WORKDIR /app

# 5. Expose SSH port
EXPOSE 22

# Start the SSH service
CMD ["/usr/sbin/sshd", "-D"]
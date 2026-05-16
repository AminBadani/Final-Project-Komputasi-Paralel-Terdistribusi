FROM ubuntu:24.04

# 1. Install dependencies
# We need build-essential for GCC/OpenMP and OpenMPI for distributed logic
RUN apt-get update && apt-get install -y \
    build-essential \
    openmpi-bin \
    libopenmpi-dev \
    openssh-server \
    openssh-client \
    && rm -rf /var/lib/apt/lists/*

# 2. Setup the SSH directory for root
RUN mkdir -p /root/.ssh && chmod 700 /root/.ssh

# 3. Generate an RSA Key Pair with NO password (passphrase "")
RUN ssh-keygen -t rsa -N "" -f /root/.ssh/id_rsa

# 4. Authorize the public key to trust ITSELF
# Since all containers are built from this exact same image, 
# they will all share this key and trust each other!
RUN cp /root/.ssh/id_rsa.pub /root/.ssh/authorized_keys && chmod 600 /root/.ssh/authorized_keys

# 5. Disable Strict Host Key Checking (the yes/no prompt)
RUN echo "    StrictHostKeyChecking no" >> /etc/ssh/ssh_config

# 6. Configure the SSH daemon to allow root login via keys
RUN mkdir /var/run/sshd
RUN echo "PermitRootLogin yes" >> /etc/ssh/sshd_config
RUN echo "PubkeyAuthentication yes" >> /etc/ssh/sshd_config

# FIXES FOR UBUNTU 24.04 PAM & LOGIN BLOCKS:
RUN sed -i 's/UsePAM yes/UsePAM no/g' /etc/ssh/sshd_config
RUN echo "AuthorizedKeysFile .ssh/authorized_keys" >> /etc/ssh/sshd_config
RUN echo "root:root" | chpasswd

# --allow-run-as-root
ENV OMPI_ALLOW_RUN_AS_ROOT=1
ENV OMPI_ALLOW_RUN_AS_ROOT_CONFIRM=1

# 7. Set working directory
WORKDIR /app

# 8. Expose SSH port
EXPOSE 22

# Start the SSH service
CMD ["/usr/sbin/sshd", "-D"]
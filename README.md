# CSSE4011-Uranus-Violet
CSSE4011 Project Repository | Yangyang Li and Milind Bordia

Chosen Project: Copper 2 - Transmit camera image via Infrared (IR) link

Project Description: A serial camera connected to a nRF52DK board will be used to provide an image. This image will be transmitted via an IR Transmitter to an IR Receiver connected to a M5Core2 (using Zephyr). 
A suitable protocol for transmitting information over IR will be designed. Transmission will be attempted at a separation distance of 5m or more.

Team Member Roles:
Yangyang is responsible for programming the M5Core2 using Zephyr. He is also responsible for ensuring that the image can be received at the 5m distance. Milind is responsible for programming the nRF52DK board. He will ensure that the board can take an image from the serial camera and that the IR transmitter can successfully transmit the processed data. They will both work together to establish a communication protocol which will be implemented on both boards. This protocol will include error checking and will be decided as the project progresses.

Project KPIs:
1. Transmission Success Rate: Measures the percentage of successful data transmission from the nRF52DK to the M5Core2. This KPI assesses the reliability of the communication protocol and the effectiveness of the hardware setup in achieving successful data transmission.
2. Transmission Distance: Measures the maximum distance from which the image can be transmitted successfully (~90% accuracy). This KPI assesses the practical range of the transmitter and receiver system and its ability to maintain reliable transmission over distance.
3. Transmission Time: Measures the time taken to complete the transmission of a single image from the nRF52DK to the M5Core2. This KPI assesses the efficiency of the communication protocol and how viable/practical the transmission method is.
4. Protocol Overhead: Measures the amount of additional data transmitted along with the image data such as hamming bits or parity bits. This KPI assesses the efficiency of the communication protocol and the effect of additional data.
5. Error Rate: Measures the frequency of transmission errors encountered during the communication process such as bit errors or timing errors. This KPI assesses the reliability of the communication system and the hardware setup.

Block Diagrams: 

The following image is a hardware block diagram showing the flow of data through various hardware and their connections.

![4010project drawio](https://github.com/MilindianaJone/CSSE4011-Uranus-Violet/assets/167830538/96271cb9-49c9-426e-aa57-05e5fd4f7e95)



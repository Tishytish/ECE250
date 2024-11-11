.section .text
.align 2
.globl _start


// -------------------------------------------------------------------------------------
// Name:        Lab_3.S
// Purpose:     This code has 2 parts: the first part is to create a simple counter
//              subroutine that increments from 0x00 to 0xFF, wraps to 0 and continues
//              counting.  There is a 100ms delay between the display of the count. 
//      The second part is a reflex meter that will measure how fast
//              a user responds to an event accurate to a 10th of a millisecond.
//              Initially, all LED's are off and after a random amount of time
//              (between 2 and 10 seconds), one LED turns on (LED_6) and then the user
//      presses pushbutton S1.  The press of the button will be monitored by
//      using "polling".
//
// Author:      Julius Olajos
// -------------------------------------------------------------------------------------


_start:

// -------------------------------------------------------------------------------------
// GPIO Control Registers Memory Mapping

    .equ GPIO_BASE_CTRL_ADDR, 0x10012000    // Base address for the GPIO control registers
    .equ GPIO_OUTPUT_EN,      0x08          // address offset for enabling GPIO outputs
    .equ GPIO_OUTPUT_VAL,     0x0C          // Address offset for writing to GPIO outputs
    .equ GPIO_OUTPUT_XOR,     0x40          // Address offset for GPIO Outputs XOR
    .equ GPIO_INPUT_VAL,      0x00          // Address offset for reading GPIO inputs
    .equ GPIO_INPUT_EN,       0x04          // address offset for enabling GPIO inputs
    .equ GPIO_PUE,            0x10          // address offset for internal GPIO pull-up resistor

// -------------------------------------------------------------------------------------
// 8 LEDS, 7 Segment LED Display Pins Register Address Mapping

    .equ GPIO_7SEGLED_PINS,   0x0000023F      // Seven Segment LED Display Pins (7)
    .equ GPIO_LEDBAR_PINS,    0x00FC0C00      // LED Bar Pins (8)
    .equ GPIO_ALL_LED_PINS,   0x00FC0E3F      // All LED Pins (15)
    .equ GPIO_LEDBAR_LED_1,   0x00000800      // LEDBAR LED1

// -------------------------------------------------------------------------------------
// Pushbuttons SW(x) Register Address Mapping

    .equ GPIO_SW_1,           0x00001000      // SW1 pushbutton (bit 12)
    .equ GPIO_SW_2,           0x00002000      // SW2 Pushbutton (bit 13)
    .equ GPIO_ALL_PBS,        0x00003000      // All Pushbutton Pins (bits 12, 13)

// Initialize the GPIO control registers
// -------------------------------------------------------------------------------------

    li t0, GPIO_BASE_CTRL_ADDR          // Load Base Address for GPIO Control Registers
    li t1, GPIO_ALL_LED_PINS            // Load GPIO Register to set GPIO_OUTPUT_EN and GPIO_OUTPUT_XOR registers for all GPIO LED Pins
    sw t1, GPIO_OUTPUT_EN(t0)           // Enable outputs on all GPIO LED Pins
    li t2, 0xFF03F1C0
    sw t2, GPIO_OUTPUT_VAL(t0)          // Set all LED pins to zero to turn off all LEDS.
    li t1, GPIO_SW_1                    // Load GPIO Register to set GPIO_INPUT_EN for input pins
    sw t1, GPIO_INPUT_EN(t0)            // Enable inputs on all Pushbutton pins

// -------------------------------------------------------------------------------------


INITIAL:

// Initialize random number generator
    li a7, 0xABCD                   // Initializes register a7 to a 16-bit non-zero value and NOTHING else can write to a7 !!!!
    loop10:
     jal ra, RandomNum   //calls Random number subroutine
     li t4, 20000  //loads 20000 into register t4
     blt a7, t4, loop10  //branches back to call RandomNum if the random num generated is less than 20000
     li t3, 100000  //loads 100000 into register t3
     blt t3, a7, loop10  //branches back to call RandomNum if the number generated is greater than 100000

    mv a0, a7  //move the Random num that is between 20000 and 100000 +-5% into ao
    loop11:
    jal ra, DELAY //calls the 0.1 ms delay
    addi a0, a0, -1  // this reduces random Num by 1
    bnez a0, loop11  // goes back to loop if RandomNum isn't 0(to ensure 0.1 millisecond delay is called nRandomNum number of times)
        
    li t3, 0xFF43F1C0  //Loads the number that accesses the LED6, and turns it on into register t3
    sw t3, GPIO_OUTPUT_VAL(t0) //turn LED6 on
    



/*COUNTER:
     addi sp, sp, -24  //setting up stack to save register values so they aren't manipulated in a way that affects code when they return
     sw ra, 0(sp)
     sw s2, 4(sp)
     sw s1, 8(sp)
     sw t4, 12(sp)
     sw t5, 16(sp)
     sw t2, 20(sp)


     li s1, 0x0   // load 0x00 into s1
     li s2, 0xFF  //load 0xFF into s2
     loop1:
         sw t4, 12(sp)
         sw t5, 16(sp)
         sw t2, 20(sp)
         jal ra, DISPLAY_NUM  //Jumps to the subroutine that displays the each number
         lw t4, 12(sp)//restores the stack
         lw t5, 16(sp)
         lw t2, 20(sp)
         jal ra, DELAY0  //calls a 100ms delay between each number
         addi s1, s1, 1
         beq s1, s2, reset  //resets the number in s1 to zero when s1 and s2 become equal
         j loop1

    reset:
         li s1, 0x0  // sets s1 to 0x0
         j loop1  //jumps to loop1 to repeat the process of displaying counter numbers*/




COUNTER2:

    addi sp, sp, -20    //setting up stack to save register values so they aren't manipulated in a way that affects code when they return
    sw ra, 0(sp)
    sw s1, 8(sp)
    sw s2, 12(sp)
    

    li s1, 0x0    //sets s1 to 0
    loop2:


        jal ra, DELAY  //calls 0.1ms delay
        
        lw s3, GPIO_INPUT_VAL(t0) // checking if push button is pushed(i.e it is equal to 0) 
        slli s3, s3, 19 //shifts s3 by 19 to get the bit of the pushbutton, to the 
        li s2, 0x0 //loads 0 to s2
        or s2, s2, s3 //ors s2 with s3 to check if the push botton was pushed
        beqz s2, display  //branches to display if s2 is equal to 0(ie pushbutton has been pressed)

        addi s1, s1, 1  //increments s1 which is our counter by 1
        j loop2   //jumps back to loop2, to repeat the process

    display:
        li t3, 0xFF03F1C0   //loads this number that turns led 6 off into register t3
        sw t3, GPIO_OUTPUT_VAL(t0)  //turns LED6 off
        sw t0, 16(sp)
        sw t1, 20(sp)
        jal ra, DISPLAY  //calls subroutine DISPLAY
        lw t0, 16(sp)  //restores the stack
        lw t1, 20(sp)


    



; //loop:
; //  Generate random number
;         jal ra, RandomNum

//  Once your number has been generated, it will be stored in register a7. Do not overwrite this!
//  You will now need to set up delays, subroutines to turn the LED ON and OFF, and for polling whether the Pushbutton S1 has been pressed.
//  Keep in mind that you will get a 32-bit number back from the random number generator.  You will need to display this 8 bits at a time,
//  starting with the lowest 8 bits.
//  You will probably want to rotate 8 bits at a time for display.



//   --------------- Place your code here ----------------------------------------



DISPLAY:    

// Display the 32-bit number on the 8-LED Bar

// Register t1 holds the value of the number to be displayed
// Map each bit of the number to the corresponding LED Bar bit
// MSB to LSB: bits 23, 22, 21, 20, 19, 18, 11, 10

    addi sp, sp, -24 //setting up stack to save register values so they aren't manipulated in a way that affects code when they return
    li t3, 4  // loads t3 with 4 because we need to display 8 bits(from the least significant 8 bits to the most significant 8 bits) ie 4 times to display 32 nits
    mv t1, s1  //mv s1(our counter value) to t1 
    sw s1, 20(sp)
    sw s2, 16(sp)


    loop3:
        beqz t3, reset2 //jumps to reset if t3 is equal to 0 
        sw t4, 0(sp)
        sw t5, 4(sp)
        sw t2, 8(sp)
        jal ra, DISPLAY_NUM  //calls the subroutine display_num
        lw t4, 0(sp)
        lw t5, 4(sp)
        lw t2, 8(sp)
        jal ra, DELAY2  //calls the subroutine DELAY2
        srli s1, s1, 8 //shift our counter value by 8 to get the next least significant 8 bits
        addi t3, t3, -1 //reduces t3 by 1 because 1set of 8 bits has been displayed
        j loop3  //jumps back to loop3
    reset2:  //resets all values when the 32 bits have been displayed
        mv s1, t1  //moves t1(our original value) into s1
        li t3, 4 //loads 4 into t3
        jal ra, DELAY3  //calls delay 3 which is a 5 second delay between each full display of the 32 bit number
        j loop3 //jumps to loop3

    lw s1, 20(sp)  //restores the stack
    lw s2, 16(sp)
    add sp, sp, 24







DISPLAY_NUM:
      addi sp, sp, -8 //setting up stack to save register values so they aren't manipulated in a way that affects code when they return
      sw s1, 4(sp)
      sw ra, 0(sp)


// Remember, the 8 LEDS are mapped to the GPIO as bits MSB to LSB: bits 23, 22, 21, 20, 19, 18, 11, 10

    li t4, 0xFC0000  // setting up a mask that has one in positions 23-18
    slli s1, s1, 16 //shifting the bits stored in our counter to have bit 7-2 in positions 23-18
    and t5, t4, s1  //and s1(the shifted counter value) and t4(the mask) and store it into t5

    li t4, 0xC00  //setting up another mask that has 1 in bit position 11 and 10
    srli s1, s1, 6  // shifting the bits stored in our counter to have bit 1-0 in positions 11-10
    and t2, s1, t4 //and the shifted value with the mask
    add t5, t5, t2 //and this to t5 that has 7-2 in position 23-18 already

    sw t5, GPIO_OUTPUT_VAL(t0)  //store unto the output pins

      

      lw ra, 0(sp) //restoring the stack
      lw s1, 4(sp)
      addi sp, sp, 8
      ret //return

// Random Number generator
// Register a7 holds the random number.
// You can read this value anywhere in the code, but this register can only be written to by this subroutine
// Or Else!



RandomNum:
     addi sp, sp, -16 
     sw ra, 12(sp)


     li s4, 0x8000           // Load upper 20 bits
     and t1, a7, s4              // Mask to lower 16 bits
     li s4, 0x2000               // Load upper 20 bits
     and t3, a7, s4              // Mask to lower 16 bits


     slli t3, t3, 2
     xor t3, t1, t3
     li s4, 0x1000               // Load upper 20 bits
     and t1, a7, t0          // Mask to lower 16 bits

     slli t1, t1, 3
     xor t3, t3, t1
     andi t1, a7, 0x0400
     slli t1, t1, 5
     xor t3, t3, t1
     srli t3, t3, 15
     slli a7, a7, 1
     or a7, a7, t3           // Register a7 holds the random number

     lw ra, 12(sp)
     addi sp, sp, 16
     ret


//100ms delay
DELAY0:
      addi sp, sp, -16 //setting up stack to save register values so they aren't manipulated in a way that affects code when they return
      sw ra, 12(sp)
      sw s0, 8(sp)
    

     li s0, 0xF4240 //loading 0xf4240 into s0

     loop4:
         addi s0, s0, -1 //decrementing value in s0 by 1
         bnez s0, loop4 //jumping back to loop 4 to decrement it by 1 again

      lw ra, 12(sp)  //restores the stack
      lw s0, 8(sp)
      addi sp, sp, 16
      ret

// 0.1  ms delay
DELAY:
     addi sp, sp, -16  //setting up stack to save register values so they aren't manipulated in a way that affects code when they return
     sw ra, 12(sp)
     sw s0, 8(sp)
    
    li s0, 0x320  //loading 0x320 into s0
    loop5:
        addi s0, s0, -1 //decrementing value in s0 by 1
        bnez s0, loop5  //jumping back to loop 5 to decrement it by 1 again

     lw ra, 12(sp)   //restores the stack
     lw s0, 8(sp)
     addi sp, sp, 16
     ret

// 2 second delay
DELAY2:
     addi sp, sp, -16  //setting up stack to save register values so they aren't manipulated in a way that affects code when they return
     sw ra, 12(sp)
     sw s0, 8(sp)
    

    li s0, 0xF42400  //loading 0xf42400 into s0

    loop6:
        addi s0, s0, -1 //decrementing value in s0 by 1
        bnez s0, loop6  //jumping back to loop 6 to decrement it by 1 again

     lw ra, 12(sp)   //restores the stack
     lw s0, 8(sp)
     addi sp, sp, 16
     ret

// 5 second delay
DELAY3:
    addi sp, sp, -16  //setting up stack to save register values so they aren't manipulated in a way that affects code when they return
    sw ra, 12(sp)
    sw s0, 8(sp)
    

    li s0, 0x2625A00  //loading 0x2625A00 into s0

    loop7:
        addi s0, s0, -1 //decrementing value in s0 by 1
        bnez s0, loop7  //jumping back to loop 7 to decrement it by 1 again

    lw ra, 12(sp)   //restores the stack
    lw s0, 8(sp)
    addi sp, sp, 16
    ret
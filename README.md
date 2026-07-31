# LSB Image Steganography

A command-line C application that hides (steganographs) a secret text file inside a `.bmp` image using **LSB (Least Significant Bit)** steganography, and can later extract it back out.


## Abstract

Steganography is the art of hiding the fact that communication is taking place, by concealing information inside other information. Unlike cryptography  which hides the *content* of a message  steganography hides the *existence* of the message itself. Digital images are one of the most popular carriers for this because of how common they are on the internet.

This project exploits the fact that human perception can't detect tiny changes made to the least significant bits of pixel data, allowing a secret file to be embedded inside a `.bmp` image without any visibly noticeable change.

## Features

- Accepts a `.bmp` image along with a `.txt` file containing the secret message
- Validates that the image has enough capacity to hold the secret message before encoding
- Embeds a **magic string** signature so a steged image can later be identified as containing hidden data
- Supports both **encoding** and **decoding**
- Fully command-line driven  all options passed as arguments

## Prerequisites

To understand and build this project, you should be familiar with:

- Basics of encryption and decryption
- File I/O in C
- Pointers and structures in C

## Usage

```
lsb_steg: Encoding: lsb_steg -e <.bmp file> <.txt file> [output file]
lsb_steg: Decoding: lsb_steg -d <.bmp file> [output file]
```

| Flag | Purpose |
|------|---------|
| `-e` | Encode a secret `.txt` file into a `.bmp` image |
| `-d` | Decode a secret file previously hidden in a `.bmp` image |

If no output file name is provided, default names are used (`steged_img.bmp` for encoding, `decoded.txt` for decoding).

### Examples

**Encode with default output name:**
```
$ ./lsb_steg -e beautiful.bmp secret.txt
INFO: Output File not mentioned. Creating steged_img.bmp as default
...
INFO: ## Encoding Done Successfully ##
```

**Encode with a specified output name:**
```
$ ./lsb_steg -e beautiful.bmp secret.txt steged_beautiful.bmp
...
INFO: ## Encoding Done Successfully ##
```

**Decode with default output name:**
```
$ ./lsb_steg -d steged_beautiful.bmp
INFO: Output File not mentioned. Creating decoded.txt as default
...
INFO: ## Decoding Done Successfully ##
```

**Decode with a specified output name:**
```
$ ./lsb_steg -d steged_beautiful.bmp secret_msg.txt
...
INFO: ## Decoding Done Successfully ##
```

## Design / Flow

**Encoding (`-e`):**
1. Verify the carrier file is a `.bmp`
2. Verify a secret file has been provided
3. Check whether an output file name was given (else create a default one)
4. Check whether the `.bmp` image's data section has enough capacity for the secret file
5. Copy the BMP header to the output file
6. Encode, in order:
   - Magic string signature
   - Secret file's extension
   - Secret file's size
   - Secret file's data
7. Copy over any remaining/unused image data
8. Done

**Decoding (`-d`):**
1. Verify the carrier file is a `.bmp`
2. Check whether an output file name was given (else create a default one)
3. Decode, in order:
   - Magic string signature (to confirm the image is actually steged)
   - Secret file's extension
   - Secret file's size
   - Secret file's data
4. Write the recovered data to the output file
5. Done

If required arguments are missing or an invalid file type is provided, the tool prints a usage/error message and exits.

## Encoding Layout

Data is embedded sequentially into the BMP's pixel data (after the header) in this order:

1. Magic string (signature marker)
2. Original secret file's extension
3. Original secret file's size
4. Original secret file's raw data

This layout allows the decoder to first confirm the image is steged, then know exactly how much data to extract and what type it originally was.

## References

- [Steganography — Wikipedia](https://en.wikipedia.org/wiki/Steganography)
- [BMP file format — Wikipedia](https://en.wikipedia.org/wiki/BMP)

---

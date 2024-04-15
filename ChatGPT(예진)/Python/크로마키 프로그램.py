from PIL import Image

def chroma_key_compositing(chromakey_path, background_path, output_path):
    # Load images
    chromakey_img = Image.open(chromakey_path)
    background_img = Image.open(background_path)

    # Convert images to RGBA mode
    chromakey_img = chromakey_img.convert('RGBA')
    background_img = background_img.convert('RGBA')

    # Get pixel data
    chromakey_data = chromakey_img.getdata()
    background_data = background_img.getdata()

    # New image to store composited result
    result_img = Image.new('RGBA', chromakey_img.size)

    # Perform chroma key compositing
    for i in range(len(chromakey_data)):
        r, g, b, a = chromakey_data[i]
        if g > r*1.1 and g > b*1.1:  # Assuming green screen with some tolerance
            result_img.putpixel((i % chromakey_img.width, i // chromakey_img.width), background_data[i])
        else:
            result_img.putpixel((i % chromakey_img.width, i // chromakey_img.width), (r, g, b, a))

    # Save result
    result_img.save(output_path)

# Paths to input and output files
chromakey_path = 'chromakey.png'
background_path = 'background.png'
output_path = 'output_image.png'  # Change the output image name if needed

# Perform chroma key compositing
chroma_key_compositing(chromakey_path, background_path, output_path)

import tensorflow as tf
from tensorflow.keras import layers, models
from tensorflow.keras.datasets import cifar10
from tensorflow.keras.utils import to_categorical
from tensorflow.keras.preprocessing.image import ImageDataGenerator
from tensorflow.keras.callbacks import EarlyStopping

def residual_block(x, filters, strides=1, downsample=False):
    y = layers.Conv2D(filters, kernel_size=3, strides=strides, padding='same')(x)
    y = layers.BatchNormalization()(y)
    y = layers.ReLU()(y)
    y = layers.Conv2D(filters, kernel_size=3, padding='same')(y)
    y = layers.BatchNormalization()(y)

    if downsample:
        x = layers.Conv2D(filters, kernel_size=1, strides=strides, padding='same')(x)
        x = layers.BatchNormalization()(x)

    out = layers.add([x, y])
    out = layers.ReLU()(out)
    return out

def create_resnet(input_shape, num_classes):
    inputs = layers.Input(shape=input_shape)

    x = layers.Conv2D(64, kernel_size=7, strides=2, padding='same')(inputs)
    x = layers.BatchNormalization()(x)
    x = layers.ReLU()(x)
    x = layers.MaxPooling2D(pool_size=3, strides=2, padding='same')(x)

    num_blocks_list = [2, 2, 2, 2]
    filters = 64
    for i, num_blocks in enumerate(num_blocks_list):
        strides = 1
        if i != 0:
            strides = 2
        x = residual_block(x, filters, strides=strides)
        for _ in range(num_blocks-1):
            x = residual_block(x, filters)
        filters *= 2

    x = layers.GlobalAveragePooling2D()(x)
    outputs = layers.Dense(num_classes, activation='softmax')(x)

    model = models.Model(inputs, outputs)
    return model

(x_train, y_train), (x_test, y_test) = cifar10.load_data()
input_shape = x_train.shape[1:]
num_classes = len(set(y_train.flatten()))

x_train = x_train.astype('float32') / 255.0
x_test = x_test.astype('float32') / 255.0
y_train = to_categorical(y_train, num_classes)
y_test = to_categorical(y_test, num_classes)

datagen = ImageDataGenerator(rotation_range=15, width_shift_range=0.1, height_shift_range=0.1, horizontal_flip=True)
datagen.fit(x_train)

model = create_resnet(input_shape, num_classes)
model.compile(optimizer='adam', loss='categorical_crossentropy', metrics=['accuracy'])

early_stopping = EarlyStopping(monitor='val_loss', patience=3, restore_best_weights=True)

batch_size = 128
epochs = 10
history = model.fit(datagen.flow(x_train, y_train, batch_size=batch_size),
                    steps_per_epoch=len(x_train) // batch_size, epochs=epochs,
                    validation_data=(x_test, y_test), callbacks=[early_stopping])

test_loss, test_acc = model.evaluate(x_test, y_test, verbose=2)
print("Test Accuracy:", test_acc)
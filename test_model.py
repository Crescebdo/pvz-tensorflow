import tensorflow as tf

img_height = 100
img_width = 80
batch_size = 2
ds_validation = tf.keras.preprocessing.image_dataset_from_directory(
    r"C:\Users\Chenz\pvz-tensorflow\data\data2",
    labels="inferred",
    label_mode="int",  # categorical, binary
    # class_names=['0', '1', '2', '3', ...]
    color_mode="rgb",
    batch_size=batch_size,
    image_size=(img_height, img_width),  # reshape if not in this size
    shuffle=True,
    seed=123,
    validation_split=0.2,
    subset="validation",
)

new_model = tf.keras.models.load_model(r"C:\Users\Chenz\pvz-tensorflow\saved_model\model1")
loss, acc = new_model.evaluate(ds_validation, verbose=2)
print('Restored model, accuracy: {:5.2f}%'.format(100 * acc))

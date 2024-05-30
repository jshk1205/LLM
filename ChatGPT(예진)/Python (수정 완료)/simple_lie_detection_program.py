import re
import pandas as pd
import nltk
from nltk.corpus import stopwords
from nltk.stem import PorterStemmer
from nltk.tokenize import word_tokenize
from sklearn.feature_extraction.text import TfidfVectorizer
from sklearn.ensemble import RandomForestClassifier

# 거짓말 탐지 프로그램을 만들라 했더니 이런 식으로 만들어주었는데
# 거짓말과 거짓말이 아닌 것의 기준을 잘 모르겠습니다..
# 학습 데이터와 같거나 비슷한 말을 입력시키면 진실, 아니면 거짓이라고 뜨는 프로그램입니다..

# 아래는 지피티의 답변입니다
# 프로그램에서는 주어진 데이터를 바탕으로 "거짓"과 "진실"을 구분하는 모델을 학습합니다. 이 모델은 텍스트 데이터를 입력으로 받아 그것이 거짓인지 진실인지를 판단합니다. 학습 데이터에서는 라벨이 주어져 있기 때문에, 이를 기반으로 모델이 거짓과 진실을 구분하도록 학습됩니다. 일반적으로 이러한 모델은 주어진 데이터에서 학습한 패턴을 기반으로 새로운 문장이 거짓인지 여부를 예측합니다.

import pandas as pd
import re
import nltk
from nltk.corpus import stopwords
from nltk.stem import PorterStemmer
from nltk.tokenize import word_tokenize
from sklearn.feature_extraction.text import TfidfVectorizer
from sklearn.ensemble import RandomForestClassifier

# Text preprocessing function definition
def preprocess_text(text):
    # Convert to lowercase
    text = text.lower()
    # Remove punctuation
    text = re.sub(r'[^\w\s]', '', text)
    # Remove stopwords
    stop_words = set(stopwords.words('english'))
    word_tokens = word_tokenize(text)
    text = ' '.join([word for word in word_tokens if word not in stop_words])
    # Stemming
    ps = PorterStemmer()
    words = word_tokenize(text)
    text = ' '.join([ps.stem(word) for word in words])
    return text

# Load the model
def load_model():
    # Train the model with given data
    data = {
        "sentence": ["He always speaks the truth.", "Their story was false.", "This message contains false information."],
        "label": [0, 1, 1]
    }
    df = pd.DataFrame(data)

    # Define functions to generate fake and true sentences
    def generate_fake_sentences(num_samples):
        fake_sentences = []
        for _ in range(num_samples):
            # Generate a fake sentence
            fake_sentence = "They always tell lies."  # Using a fixed sentence as an example
            fake_sentences.append(fake_sentence)
        return fake_sentences

    def generate_true_sentences(num_samples):
        true_sentences = []
        for _ in range(num_samples):
            # Generate a true sentence
            true_sentence = "He always speaks the truth."  # Using a fixed sentence as an example
            true_sentences.append(true_sentence)
        return true_sentences

    # Generate more fake and true sentences
    num_fake_samples = 1000
    num_true_samples = 1000
    fake_sentences = generate_fake_sentences(num_fake_samples)
    true_sentences = generate_true_sentences(num_true_samples)

    # Add the generated fake and true sentences to the DataFrame
    fake_df = pd.DataFrame({'sentence': fake_sentences, 'label': [1] * num_fake_samples})
    true_df = pd.DataFrame({'sentence': true_sentences, 'label': [0] * num_true_samples})
    df = pd.concat([df, fake_df, true_df], ignore_index=True)

    # Apply text preprocessing
    df['sentence'] = df['sentence'].apply(preprocess_text)

    # Separate input data (X) and output labels (y)
    X = df['sentence']
    y = df['label']

    # TF-IDF vectorization
    vectorizer = TfidfVectorizer()
    X_vectorized = vectorizer.fit_transform(X)

    # Train the model using the Random Forest classifier
    model = RandomForestClassifier(n_estimators=100, random_state=42)
    model.fit(X_vectorized, y)

    return vectorizer, model

def predict_lie(sentence, vectorizer, model):
    # Apply text preprocessing
    sentence = preprocess_text(sentence)
    # TF-IDF vectorization
    sentence_vectorized = vectorizer.transform([sentence])
    # Predict the label of the sentence using the model
    prediction = model.predict(sentence_vectorized)
    if prediction[0] == 0:
        return "This sentence is true."
    else:
        return "This sentence is false."

if __name__ == "__main__":
    # Load the model
    vectorizer, model = load_model()

    # Input the test sentence
    test_sentence = input("Enter the sentence to test: ")

    # Determine if it's a lie
    result = predict_lie(test_sentence, vectorizer, model)
    print(result)

import pandas as pd
from sklearn.model_selection import train_test_split
from sklearn.feature_extraction.text import TfidfVectorizer
from sklearn.ensemble import RandomForestClassifier
from sklearn.metrics import accuracy_score, classification_report
from nltk.corpus import stopwords
from nltk.stem import PorterStemmer
from nltk.tokenize import word_tokenize
import re
import random
import nltk
nltk.download('punkt')

# 주어진 데이터를 DataFrame으로 변환
data = {
    "sentence": ["그는 항상 진실을 말한다.", "그들의 이야기는 거짓이었다.", "이 메시지는 거짓 정보를 포함하고 있습니다."],
    "label": [0, 1, 1]
}
df = pd.DataFrame(data)

# 거짓과 진실 문장 생성 함수 정의
def generate_fake_sentences(num_samples):
    fake_sentences = []
    for _ in range(num_samples):
        # 거짓 문장 생성
        fake_sentence = "그들은 항상 거짓을 말합니다."  # 예시로 고정된 문장 사용
        fake_sentences.append(fake_sentence)
    return fake_sentences

def generate_true_sentences(num_samples):
    true_sentences = []
    for _ in range(num_samples):
        # 진실 문장 생성
        true_sentence = "그는 항상 진실을 말합니다."  # 예시로 고정된 문장 사용
        true_sentences.append(true_sentence)
    return true_sentences

# 더 많은 거짓과 진실 문장 생성
num_fake_samples = 1000
num_true_samples = 1000
fake_sentences = generate_fake_sentences(num_fake_samples)
true_sentences = generate_true_sentences(num_true_samples)

# 생성된 거짓과 진실 문장을 데이터프레임에 추가
fake_df = pd.DataFrame({'sentence': fake_sentences, 'label': [1]*num_fake_samples})
true_df = pd.DataFrame({'sentence': true_sentences, 'label': [0]*num_true_samples})
df = pd.concat([df, fake_df, true_df], ignore_index=True)

# 텍스트 전처리 함수 정의
def preprocess_text(text):
    # 소문자 변환
    text = text.lower()
    # 구두점 제거
    text = re.sub(r'[^\w\s]', '', text)
    # 불용어 제거
    stop_words = set(stopwords.words('english'))
    word_tokens = word_tokenize(text)
    text = ' '.join([word for word in word_tokens if word not in stop_words])
    # 어간 추출
    ps = PorterStemmer()
    words = word_tokenize(text)
    text = ' '.join([ps.stem(word) for word in words])
    return text

# 텍스트 전처리 적용
df['sentence'] = df['sentence'].apply(preprocess_text)

# 입력 데이터(X)와 출력 레이블(y) 분리
X = df['sentence']
y = df['label']

# TF-IDF 벡터화
vectorizer = TfidfVectorizer()
X_vectorized = vectorizer.fit_transform(X)

# 전체 데이터를 훈련 데이터와 테스트 데이터로 분할 (80% 훈련, 20% 테스트)
X_train, X_test, y_train, y_test = train_test_split(X_vectorized, y, test_size=0.2, random_state=42)

# 랜덤 포레스트 분류기를 사용하여 모델 학습
model = RandomForestClassifier(n_estimators=100, random_state=42)
model.fit(X_train, y_train)

# 모델 평가
y_pred = model.predict(X_test)
accuracy = accuracy_score(y_test, y_pred)
print("모델 정확도:", accuracy)
print("Classification Report:\n", classification_report(y_test, y_pred))

# 입력된 문장이 거짓인지 여부를 판단하는 함수 정의
def predict_lie(sentence):
    # 텍스트 전처리 적용
    sentence = preprocess_text(sentence)
    # TF-IDF 벡터화
    sentence_vectorized = vectorizer.transform([sentence])
    # 모델로 문장의 레이블 예측
    prediction = model.predict(sentence_vectorized)
    if prediction[0] == 0:
        return "이 문장은 진실입니다."
    else:
        return "이 문장은 거짓입니다."

# 테스트 문장에 대한 거짓말 여부 출력
test_sentence = "그들은 언제나 정직합니다."
print(predict_lie(test_sentence))

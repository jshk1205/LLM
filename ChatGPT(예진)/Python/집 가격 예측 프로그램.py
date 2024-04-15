import numpy as np
from sklearn.tree import DecisionTreeRegressor
from sklearn.ensemble import RandomForestRegressor, GradientBoostingRegressor
from sklearn.svm import SVR
from sklearn.naive_bayes import GaussianNB
from sklearn.neighbors import KNeighborsRegressor
from xgboost import XGBRegressor

class LinearRegression:
    def __init__(self):
        self.weights = None
        self.bias = None

    def fit(self, X, y):
        n_samples, n_features = X.shape

        # 초기화: weights와 bias를 0으로 설정
        self.weights = np.zeros(n_features)
        self.bias = 0

        # 최소 제곱법을 사용하여 모델 학습
        X_transpose = np.transpose(X)
        A = np.dot(X_transpose, X)
        b = np.dot(X_transpose, y)
        self.weights = np.dot(np.linalg.inv(A), b)

    def predict(self, X):
        return np.dot(X, self.weights) + self.bias

class NeuralNetwork:
    def __init__(self, layers=[10, 5, 1], learning_rate=0.01, n_iterations=1000):
        self.layers = layers
        self.learning_rate = learning_rate
        self.n_iterations = n_iterations
        self.weights = []
        self.biases = []

        # 가중치와 편향 초기화
        for i in range(1, len(layers)):
            self.weights.append(np.random.randn(layers[i - 1], layers[i]))
            self.biases.append(np.random.randn(layers[i]))

    def _sigmoid(self, x):
        return 1 / (1 + np.exp(-x))

    def _sigmoid_derivative(self, x):
        return x * (1 - x)

    def fit(self, X, y):
        for _ in range(self.n_iterations):
            # 순전파 (forward propagation)
            activations = [X]
            layer_output = X
            for i in range(len(self.layers) - 1):
                layer_output = self._sigmoid(np.dot(layer_output, self.weights[i]) + self.biases[i])
                activations.append(layer_output)

            # 역전파 (backpropagation)
            errors = [y - layer_output]
            for i in range(len(self.layers) - 2, 0, -1):
                errors.append(np.dot(errors[-1], self.weights[i].T) * self._sigmoid_derivative(activations[i]))

            # 가중치와 편향 업데이트
            for i in range(len(self.weights)):
                self.weights[i] += self.learning_rate * np.dot(activations[i].T, errors[len(self.layers) - 2 - i]) / len(X)
                self.biases[i] += self.learning_rate * np.mean(errors[len(self.layers) - 2 - i], axis=0)

    def predict(self, X):
        layer_output = X
        for i in range(len(self.layers) - 1):
            layer_output = self._sigmoid(np.dot(layer_output, self.weights[i]) + self.biases[i])
        return layer_output

# 학습 데이터: 각 특징(feature)에 따른 집 가격
# 특징은 방의 개수, 주택 면적, 건축 연도, 위치 인덱스, 시세 변동률, 주변 시설 및 장소 유형 등으로 이루어짐
X_train = np.array([[2, 100, 1990, 0.8, 1.2, 0.5, 1, 0, 0, 1],
                    [3, 150, 1995, 0.7, 1.1, 0.6, 0, 1, 0, 1],
                    [4, 200, 2000, 0.9, 1.0, 0.7, 1, 0, 1, 0],
                    [5, 250, 2010, 0.6, 1.3, 0.8, 0, 1, 1, 0],
                    [6, 300, 2020, 0.5, 1.4, 0.9, 1, 0, 1, 1]])  # 각 집의 특징 (방의 개수, 주택 면적, 건축 연도, 위치 인덱스, 시세 변동률, 주변 시설 및 장소 유형 등)
y_train = np.array([[200], [300], [400], [500], [600]])    # 각 집의 가격 (만 달러)

# 선형 회귀 모델 학습
linear_model = LinearRegression()
linear_model.fit(X_train, y_train)

# 다층 퍼셉트론 모델 학습
nn_model = NeuralNetwork(layers=[10, 7, 5, 1], learning_rate=0.1, n_iterations=10000)
nn_model.fit(X_train, y_train)

# Decision Tree 모델 학습
dt_model = DecisionTreeRegressor()
dt_model.fit(X_train, y_train)

# Random Forest 모델 학습
rf_model = RandomForestRegressor()
rf_model.fit(X_train, y_train)

# Support Vector Machine 모델 학습
svm_model = SVR()
svm_model.fit(X_train, y_train)

# Naive Bayes 모델 학습
nb_model = GaussianNB()
nb_model.fit(X_train, y_train.ravel())  # GaussianNB는 1차원 배열 형태의 타겟을 입력으로 받음

# k-Nearest Neighbors 모델 학습
knn_model = KNeighborsRegressor()
knn_model.fit(X_train, y_train)

# Gradient Boosting 모델 학습
gb_model = GradientBoostingRegressor()
gb_model.fit(X_train, y_train.ravel())  # GradientBoostingRegressor는 1차원 배열 형태의 타겟을 입력으로 받음

# XGBoost 모델 학습
xgb_model = XGBRegressor()
xgb_model.fit(X_train, y_train.ravel())  # XGBRegressor는 1차원 배열 형태의 타겟을 입력으로 받음

# 새로운 집의 특징을 입력하여 가격 예측
X_new = np.array([[7, 350, 2025, 0.4, 1.5, 1.0, 1, 1, 0, 1]])  # 예를 들어, 방의 개수가 7개이고 주택 면적이 350이며 2025년에 건축된 집, 위치 인덱스가 0.4, 시세 변동률이 1.5, 주변 시설 및 장소 유형 등이 포함된 집

# 각 모델로 집값 예측
linear_prediction = linear_model.predict(X_new)
nn_prediction = nn_model.predict(X_new)
dt_prediction = dt_model.predict(X_new)
rf_prediction = rf_model.predict(X_new)
svm_prediction = svm_model.predict(X_new)
nb_prediction = nb_model.predict(X_new)
knn_prediction = knn_model.predict(X_new)
gb_prediction = gb_model.predict(X_new)
xgb_prediction = xgb_model.predict(X_new)

# 모델별 예측값 출력
predictions = {
    "선형 회귀": linear_prediction[0],
    "다층 퍼셉트론": nn_prediction[0][0],
    "의사결정트리": dt_prediction[0],
    "랜덤 포레스트": rf_prediction[0],
    "서포트 벡터 머신": svm_prediction[0],
    "나이브 베이즈": nb_prediction[0],
    "k-최근접 이웃": knn_prediction[0],
    "Gradient Boosting": gb_prediction[0],
    "XGBoost": xgb_prediction[0]
}

# 가장 낮은 집값과 사용된 모델 출력
min_model = min(predictions, key=predictions.get)
min_price = predictions[min_model]

print("가장 낮은 집값:", min_price, "만 달러")
print("사용된 모델:", min_model)

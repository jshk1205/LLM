import numpy as np
from sklearn.tree import DecisionTreeRegressor
from sklearn.ensemble import RandomForestRegressor, GradientBoostingRegressor
from sklearn.svm import SVR
from sklearn.naive_bayes import GaussianNB
from sklearn.neighbors import KNeighborsRegressor
from xgboost import XGBRegressor
import warnings

warnings.filterwarnings("ignore")

class LinearRegression:
    def __init__(self):
        self.weights = None
        self.bias = None

    def fit(self, X, y):
        n_samples, n_features = X.shape

        self.weights = np.zeros(n_features)
        self.bias = 0

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

        # Initialize weights and biases
        for i in range(1, len(layers)):
            self.weights.append(np.random.randn(layers[i - 1], layers[i]))
            self.biases.append(np.random.randn(layers[i]))

    def _sigmoid(self, x):
        return 1 / (1 + np.exp(-x))

    def _sigmoid_derivative(self, x):
        return x * (1 - x)

    def fit(self, X, y):
        for _ in range(self.n_iterations):
            # forward propagation
            activations = [X]
            layer_output = X
            for i in range(len(self.layers) - 1):
                layer_output = self._sigmoid(np.dot(layer_output, self.weights[i]) + self.biases[i])
                activations.append(layer_output)

            # backpropagation
            errors = [y - layer_output]
            for i in range(len(self.layers) - 2, 0, -1):
                errors.append(np.dot(errors[-1], self.weights[i].T) * self._sigmoid_derivative(activations[i]))

            for i in range(len(self.weights)):
                self.weights[i] += self.learning_rate * np.dot(activations[i].T, errors[len(self.layers) - 2 - i]) / len(X)
                self.biases[i] += self.learning_rate * np.mean(errors[len(self.layers) - 2 - i], axis=0)

    def predict(self, X):
        layer_output = X
        for i in range(len(self.layers) - 1):
            layer_output = self._sigmoid(np.dot(layer_output, self.weights[i]) + self.biases[i])
        return layer_output

X_train = np.array([[2, 100, 1990, 0.8, 1.2, 0.5, 1, 0, 0, 1],
                    [3, 150, 1995, 0.7, 1.1, 0.6, 0, 1, 0, 1],
                    [4, 200, 2000, 0.9, 1.0, 0.7, 1, 0, 1, 0],
                    [5, 250, 2010, 0.6, 1.3, 0.8, 0, 1, 1, 0],
                    [6, 300, 2020, 0.5, 1.4, 0.9, 1, 0, 1, 1]])
y_train = np.array([[200], [300], [400], [500], [600]])

linear_model = LinearRegression()
linear_model.fit(X_train, y_train)

nn_model = NeuralNetwork(layers=[10, 7, 5, 1], learning_rate=0.1, n_iterations=10000)
nn_model.fit(X_train, y_train)

dt_model = DecisionTreeRegressor()
dt_model.fit(X_train, y_train)

rf_model = RandomForestRegressor()
rf_model.fit(X_train, y_train)

svm_model = SVR()
svm_model.fit(X_train, y_train)

nb_model = GaussianNB()
nb_model.fit(X_train, y_train.ravel())

knn_model = KNeighborsRegressor()
knn_model.fit(X_train, y_train)

gb_model = GradientBoostingRegressor()
gb_model.fit(X_train, y_train.ravel())

xgb_model = XGBRegressor()
xgb_model.fit(X_train, y_train.ravel())

X_new = np.array([[7, 350, 2025, 0.4, 1.5, 1.0, 1, 1, 0, 1]])

linear_prediction = linear_model.predict(X_new)
nn_prediction = nn_model.predict(X_new)
dt_prediction = dt_model.predict(X_new)
rf_prediction = rf_model.predict(X_new)
svm_prediction = svm_model.predict(X_new)
nb_prediction = nb_model.predict(X_new)
knn_prediction = knn_model.predict(X_new)
gb_prediction = gb_model.predict(X_new)
xgb_prediction = xgb_model.predict(X_new)

predictions = {
    "linear regression": linear_prediction.ravel(),
    "Multilayer Perceptron": nn_prediction.ravel(),
    "Decision Tree": dt_prediction.ravel(),
    "Random Forest": rf_prediction.ravel(),
    "Support Vector Machine": svm_prediction.ravel(),
    "Knives Bayes": nb_prediction.ravel(),
    "k - nearest neighbor": knn_prediction.ravel(),
    "Gradient Boosting": gb_prediction.ravel(),
    "XGBoost": xgb_prediction.ravel()
}

min_model = min(predictions, key=predictions.get)
min_price = min(predictions.values())
min_price = min_price[0]

print("Various machine learning models were used to predict house prices.\n")

for model, price in predictions.items():
    if isinstance(price, np.ndarray):
        price = price[0]
    print(f"{model} : {price:.2f} thousand dollars")

print("\nThe lowest house prices and corresponding models are as follows:")
print(f"Model: {min_model}")
print(f"The lowest house price: {min_price:.2f} thousand dollars")

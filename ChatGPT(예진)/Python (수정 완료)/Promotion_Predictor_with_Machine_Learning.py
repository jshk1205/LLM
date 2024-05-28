import numpy as np
import pandas as pd
from sklearn.model_selection import train_test_split
from sklearn.preprocessing import StandardScaler, OneHotEncoder
from sklearn.feature_selection import SelectKBest, f_classif
from sklearn.decomposition import PCA
from sklearn.ensemble import RandomForestClassifier, GradientBoostingClassifier, AdaBoostClassifier
from sklearn.svm import SVC
from sklearn.pipeline import Pipeline
from sklearn.impute import SimpleImputer
from sklearn.compose import ColumnTransformer
from sklearn.metrics import accuracy_score, confusion_matrix, classification_report, roc_auc_score, roc_curve
import matplotlib.pyplot as plt

# Generate synthetic data
np.random.seed(0)
data = pd.DataFrame({
    'PerformanceRating': np.random.randint(1, 6, 1000),
    'CompetencyScore': np.random.randint(1, 101, 1000),
    'YearsOfService': np.random.randint(1, 11, 1000),
    'TeamworkSkill': np.random.randint(1, 101, 1000),
    'ProjectExecutionSkill': np.random.randint(1, 101, 1000),
    'CommunicationSkill': np.random.randint(1, 101, 1000),
    'Creativity': np.random.randint(1, 101, 1000),
    'TechnicalSkill': np.random.randint(1, 101, 1000),
    'Promotion': np.random.randint(0, 2, 1000)
})

# Separate numerical and categorical features
numeric_features = ['PerformanceRating', 'CompetencyScore', 'YearsOfService', 'TeamworkSkill', 'ProjectExecutionSkill', 'CommunicationSkill', 'Creativity', 'TechnicalSkill']
categorical_features = []

# Imputation and scaling for numerical features, encoding for categorical features
numeric_pipeline = Pipeline([
    ('imputer', SimpleImputer(strategy='median')),
    ('scaler', StandardScaler())
])

categorical_pipeline = Pipeline([
    ('imputer', SimpleImputer(strategy='most_frequent')),
    ('encoder', OneHotEncoder())
])

# Define the preprocessor pipeline
preprocessor = ColumnTransformer([
    ('num', numeric_pipeline, numeric_features),
    ('cat', categorical_pipeline, categorical_features)
])

# Feature selection and PCA
selector = SelectKBest(f_classif, k=8)
pca = PCA(n_components=7)

# Define models
models = {
    'RandomForest': RandomForestClassifier(),
    'GradientBoosting': GradientBoostingClassifier(),
    'AdaBoost': AdaBoostClassifier(),
    'SVM': SVC()
}

# Define pipelines
pipelines = {}
for name, model in models.items():
    pipelines[name] = Pipeline([
        ('preprocessor', preprocessor),
        ('selector', selector),
        ('pca', pca),
        ('classifier', model)
    ])

# Split the data
X = data.drop('Promotion', axis=1)
y = data['Promotion']
X_train, X_test, y_train, y_test = train_test_split(X, y, test_size=0.2, random_state=42)

# Train and evaluate models
for name, pipeline in pipelines.items():
    print(f"Training {name}...")
    pipeline.fit(X_train, y_train)
    y_pred = pipeline.predict(X_test)

    # Calculate evaluation metrics
    acc = accuracy_score(y_test, y_pred)
    cm = confusion_matrix(y_test, y_pred)
    report = classification_report(y_test, y_pred)
    roc_auc = roc_auc_score(y_test, y_pred)
    fpr, tpr, thresholds = roc_curve(y_test, y_pred)

    # Print results
    print(f"{name} Accuracy: {acc}")
    print(f"{name} Confusion Matrix:\n{cm}\n")
    print(f"{name} Classification Report:\n{report}\n")
    print(f"{name} ROC AUC Score: {roc_auc}\n")
    print(f"{name} ROC Curve:\n")

    # Plot ROC Curve
    plt.figure(figsize=(8, 6))
    plt.plot(fpr, tpr, label=f'ROC Curve (AUC = {roc_auc:.2f})')
    plt.plot([0, 1], [0, 1], 'k--')
    plt.xlim([0.0, 1.0])
    plt.ylim([0.0, 1.05])
    plt.xlabel('False Positive Rate')
    plt.ylabel('True Positive Rate')
    plt.title('Receiver Operating Characteristic Curve')
    plt.legend(loc='lower right')
    plt.show()

    # Calculate and print additional metrics
    sensitivity = cm[1, 1] / (cm[1, 0] + cm[1, 1])  # True Positive Rate
    specificity = cm[0, 0] / (cm[0, 0] + cm[0, 1])  # True Negative Rate
    precision = cm[1, 1] / (cm[0, 1] + cm[1, 1])    # Positive Predictive Value
    print(f"{name} Sensitivity: {sensitivity}")
    print(f"{name} Specificity: {specificity}")
    print(f"{name} Precision: {precision}\n")

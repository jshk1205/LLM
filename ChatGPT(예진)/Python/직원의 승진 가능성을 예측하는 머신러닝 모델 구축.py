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

# 가상의 데이터 생성
np.random.seed(0)
data = pd.DataFrame({
    '성과등급': np.random.randint(1, 6, 1000),
    '역량점수': np.random.randint(1, 101, 1000),
    '근속년수': np.random.randint(1, 11, 1000),
    '팀워크스킬': np.random.randint(1, 101, 1000),
    '프로젝트수행능력': np.random.randint(1, 101, 1000),
    '커뮤니케이션능력': np.random.randint(1, 101, 1000),
    '창의력': np.random.randint(1, 101, 1000),
    '기술역량': np.random.randint(1, 101, 1000),
    '승진여부': np.random.randint(0, 2, 1000)
})

# 숫자형 특성과 범주형 특성 분리
numeric_features = ['성과등급', '역량점수', '근속년수', '팀워크스킬', '프로젝트수행능력', '커뮤니케이션능력', '창의력', '기술역량']
categorical_features = []

# 결측치 처리 및 스케일링, 인코딩
numeric_pipeline = Pipeline([
    ('imputer', SimpleImputer(strategy='median')),
    ('scaler', StandardScaler())
])

categorical_pipeline = Pipeline([
    ('imputer', SimpleImputer(strategy='most_frequent')),
    ('encoder', OneHotEncoder())
])

# 파이프라인 정의
preprocessor = ColumnTransformer([
    ('num', numeric_pipeline, numeric_features),
    ('cat', categorical_pipeline, categorical_features)
])

# 특성 선택 및 PCA
selector = SelectKBest(f_classif, k=8)
pca = PCA(n_components=7)

# 모델 정의
models = {
    'RandomForest': RandomForestClassifier(),
    'GradientBoosting': GradientBoostingClassifier(),
    'AdaBoost': AdaBoostClassifier(),
    'SVM': SVC()
}

# 파이프라인 정의
pipelines = {}
for name, model in models.items():
    pipelines[name] = Pipeline([
        ('preprocessor', preprocessor),
        ('selector', selector),
        ('pca', pca),
        ('classifier', model)
    ])

# 데이터 분할
X = data.drop('승진여부', axis=1)
y = data['승진여부']
X_train, X_test, y_train, y_test = train_test_split(X, y, test_size=0.2, random_state=42)

# 모델 학습 및 평가
for name, pipeline in pipelines.items():
    print(f"Training {name}...")
    pipeline.fit(X_train, y_train)
    y_pred = pipeline.predict(X_test)

    # 평가 지표 계산
    acc = accuracy_score(y_test, y_pred)
    cm = confusion_matrix(y_test, y_pred)
    report = classification_report(y_test, y_pred)
    roc_auc = roc_auc_score(y_test, y_pred)
    fpr, tpr, thresholds = roc_curve(y_test, y_pred)

    # 결과 출력
    print(f"{name} Accuracy: {acc}")
    print(f"{name} Confusion Matrix:\n{cm}\n")
    print(f"{name} Classification Report:\n{report}\n")
    print(f"{name} ROC AUC Score: {roc_auc}\n")
    print(f"{name} ROC Curve:\n")

    # ROC Curve 그리기
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

    # 추가적인 평가 지표 계산 및 출력
    sensitivity = cm[1, 1] / (cm[1, 0] + cm[1, 1])  # 실제 True 중에 True로 예측한 비율
    specificity = cm[0, 0] / (cm[0, 0] + cm[0, 1])  # 실제 False 중에 False로 예측한 비율
    precision = cm[1, 1] / (cm[0, 1] + cm[1, 1])  # Positive로 예측한 것 중 실제 Positive의 비율
    print(f"{name} Sensitivity: {sensitivity}")
    print(f"{name} Specificity: {specificity}")
    print(f"{name} Precision: {precision}\n")

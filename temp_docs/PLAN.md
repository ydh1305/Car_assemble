# Car Assemble Refactoring Plan — OCP + Unit Test 적용

## 설계 목표

> **OCP (Open-Closed Principle)**: 새로운 부품(예: HyundaiEngine, EV 차종)을 추가할 때
> 기존 코드를 수정하지 않고(Closed), 새 클래스를 추가하는 것(Open)만으로 확장 가능한 구조.

현재 코드의 근본 문제: 차종·엔진·브레이크·조향장치가 모두 `int` 값과 `if-chain`으로만 구분되어,
부품이 추가될 때마다 `selectXxx`, `isValidCheck`, `runProducedCar`, `testProducedCar` 등
여러 곳을 동시에 수정해야 한다.

---

## 테스트 전략

- **테스트 프레임워크**: Google Test / Google Mock 1.11.0 (기존 `packages/gmock.1.11.0`)
- **테스트 실행 조건**: `_DEBUG` 빌드 → `main()` 내 `RUN_ALL_TESTS()` (기존 구조 그대로)
- **테스트 파일 위치**: `car_assemble/Tests/`
- **격리 원칙**: 각 클래스는 의존성을 Mock으로 대체해 단독 테스트

### Mock 대상
| Mock 클래스 | 용도 |
|-------------|------|
| `MockCarPrinter` | `CarAssembler` 테스트 시 출력 부수효과 제거 |
| `MockCompatibilityRule` | `CarValidator` 테스트 시 규칙을 직접 제어 |

---

## 현재 코드 문제 분석

| 문제 | 위치 | OCP 관점 |
|------|------|----------|
| 부품이 `int` + `enum` | 전역 `stack[10]` | 새 부품 추가 시 enum, stack 인덱스 변경 필요 |
| 검증 규칙이 하드코딩 | `isValidCheck()` | 규칙 추가 시 함수 본문 수정 필요 |
| 출력이 분산 | `selectXxx`, `runProducedCar` | 새 부품마다 출력 함수 수정 필요 |
| 테스트 불가 구조 | 전역 함수 + 전역 상태 | 의존성 주입 불가, Mock 불가 |
| Busy-wait delay | `delay()` L65-78 | 성능 문제 |
| 개행 처리 불명확 | L145-147 | 안전성 문제 |

---

## 목표 아키텍처

```
ICarPart (base)
├── ICarType          ← Sedan, SUV, Truck
├── IEngine           ← GMEngine, ToyotaEngine, WIAEngine, NoEngine
├── IBrakeSystem      ← MandoBrake, ContinentalBrake, BoschBrake
└── ISteeringSystem   ← BoschSteering, MobisSteering

ICompatibilityRule    ← 조합 규칙 1개 = 클래스 1개 (OCP)
CarConfig             ← ICarType*, IEngine*, IBrakeSystem*, ISteeringSystem* 보유
CarValidator          ← ICompatibilityRule 목록 순회
PartRegistry          ← 메뉴 번호 → 부품 인스턴스 팩토리
ICarPrinter           ← 출력 인터페이스 (Mock 가능하도록 분리)
CarPrinter            ← ICarPrinter 구현체 (부품의 getName() 호출)
CarAssembler          ← ICarPrinter* 주입받는 상태 머신
```

---

## Phase 1 — Method Level (main.cpp 내부)

### 1-1. `delay()` busy-wait → `Sleep()` 교체
### 1-2. 개행 문자 제거 정리 (`strcspn`)
### 1-3. `selectXxx()` if-chain → switch 변환
### 1-4. 입력 범위 검증 함수 추출 (`isInRange`)
### 1-5. `ValidationResult` 도입 및 중복 제거
### 1-6. `runProducedCar()` 스펙 출력 switch 변환

> (각 항목의 Before/After 코드는 기존 계획과 동일)

### Phase 1 Unit Tests — `Tests/Phase1_UtilTests.cpp`

`isInRange()`는 Phase 1 이후 독립 함수로 추출되므로, 아래 케이스를 테스트한다.

```cpp
// 경계값: 유효 최솟값
TEST(IsInRangeTest, CarTypeQ_MinValid)    { EXPECT_TRUE(isInRange(CarType_Q, 1)); }
TEST(IsInRangeTest, CarTypeQ_MaxValid)    { EXPECT_TRUE(isInRange(CarType_Q, 3)); }
TEST(IsInRangeTest, CarTypeQ_BelowMin)   { EXPECT_FALSE(isInRange(CarType_Q, 0)); }
TEST(IsInRangeTest, CarTypeQ_AboveMax)   { EXPECT_FALSE(isInRange(CarType_Q, 4)); }

// Engine: 0(뒤로) ~ 4(선택안함) 모두 유효
TEST(IsInRangeTest, EngineQ_BackValid)   { EXPECT_TRUE(isInRange(Engine_Q, 0)); }
TEST(IsInRangeTest, EngineQ_NoEngine)    { EXPECT_TRUE(isInRange(Engine_Q, 4)); }
TEST(IsInRangeTest, EngineQ_AboveMax)    { EXPECT_FALSE(isInRange(Engine_Q, 5)); }

// BrakeSystem
TEST(IsInRangeTest, BrakeQ_MinValid)     { EXPECT_TRUE(isInRange(brakeSystem_Q, 1)); }
TEST(IsInRangeTest, BrakeQ_MaxValid)     { EXPECT_TRUE(isInRange(brakeSystem_Q, 3)); }
TEST(IsInRangeTest, BrakeQ_AboveMax)     { EXPECT_FALSE(isInRange(brakeSystem_Q, 4)); }

// SteeringSystem
TEST(IsInRangeTest, SteeringQ_MinValid)  { EXPECT_TRUE(isInRange(SteeringSystem_Q, 1)); }
TEST(IsInRangeTest, SteeringQ_MaxValid)  { EXPECT_TRUE(isInRange(SteeringSystem_Q, 2)); }
TEST(IsInRangeTest, SteeringQ_AboveMax)  { EXPECT_FALSE(isInRange(SteeringSystem_Q, 3)); }

// Run_Test
TEST(IsInRangeTest, RunTestQ_MinValid)   { EXPECT_TRUE(isInRange(Run_Test, 0)); }
TEST(IsInRangeTest, RunTestQ_MaxValid)   { EXPECT_TRUE(isInRange(Run_Test, 2)); }
TEST(IsInRangeTest, RunTestQ_AboveMax)   { EXPECT_FALSE(isInRange(Run_Test, 3)); }
```

---

## Phase 2 — Interface Level (OCP 기반 구조)

### 2-1. `ICarPart` 기반 인터페이스

```cpp
class ICarPart {
public:
    virtual ~ICarPart() = default;
    virtual const char* getName() const = 0;
    virtual int         getId()  const = 0;
};
```

### 2-2. `ICarType` + Sedan / SUV / Truck
### 2-3. `IEngine` + GMEngine / ToyotaEngine / WIAEngine / NoEngine (`isSelected()` 포함)
### 2-4. `IBrakeSystem` + MandoBrake / ContinentalBrake / BoschBrake
### 2-5. `ISteeringSystem` + BoschSteering / MobisSteering

### Phase 2 Unit Tests — `Tests/Phase2_PartTests.cpp`

각 구현 클래스의 `getName()`, `getId()`가 spec.md §2 의 값과 일치하는지 검증.

```cpp
// --- ICarType 구현 ---
TEST(CarTypeTest, Sedan_Name)  { Sedan s; EXPECT_STREQ("Sedan", s.getName()); }
TEST(CarTypeTest, Sedan_Id)    { Sedan s; EXPECT_EQ(1, s.getId()); }
TEST(CarTypeTest, SUV_Name)    { SUV s;   EXPECT_STREQ("SUV",   s.getName()); }
TEST(CarTypeTest, SUV_Id)      { SUV s;   EXPECT_EQ(2, s.getId()); }
TEST(CarTypeTest, Truck_Name)  { Truck t; EXPECT_STREQ("Truck", t.getName()); }
TEST(CarTypeTest, Truck_Id)    { Truck t; EXPECT_EQ(3, t.getId()); }

// --- IEngine 구현 ---
TEST(EngineTest, GM_IsSelected)       { GMEngine e;      EXPECT_TRUE(e.isSelected()); }
TEST(EngineTest, Toyota_IsSelected)   { ToyotaEngine e;  EXPECT_TRUE(e.isSelected()); }
TEST(EngineTest, WIA_IsSelected)      { WIAEngine e;     EXPECT_TRUE(e.isSelected()); }
TEST(EngineTest, NoEngine_NotSelected){ NoEngine e;      EXPECT_FALSE(e.isSelected()); }
TEST(EngineTest, NoEngine_Id)         { NoEngine e;      EXPECT_EQ(4, e.getId()); }
TEST(EngineTest, GM_Id)               { GMEngine e;      EXPECT_EQ(1, e.getId()); }
TEST(EngineTest, Toyota_Id)           { ToyotaEngine e;  EXPECT_EQ(2, e.getId()); }
TEST(EngineTest, WIA_Id)              { WIAEngine e;     EXPECT_EQ(3, e.getId()); }

// --- IBrakeSystem 구현 ---
TEST(BrakeSystemTest, Mando_Name)       { MandoBrake b;       EXPECT_STREQ("Mando",       b.getName()); }
TEST(BrakeSystemTest, Continental_Name) { ContinentalBrake b; EXPECT_STREQ("Continental", b.getName()); }
TEST(BrakeSystemTest, Bosch_Name)       { BoschBrake b;       EXPECT_STREQ("Bosch",       b.getName()); }
TEST(BrakeSystemTest, Mando_Id)         { MandoBrake b;       EXPECT_EQ(1, b.getId()); }
TEST(BrakeSystemTest, Continental_Id)   { ContinentalBrake b; EXPECT_EQ(2, b.getId()); }
TEST(BrakeSystemTest, Bosch_Id)         { BoschBrake b;       EXPECT_EQ(3, b.getId()); }

// --- ISteeringSystem 구현 ---
TEST(SteeringTest, Bosch_Name)  { BoschSteering s;  EXPECT_STREQ("Bosch", s.getName()); }
TEST(SteeringTest, Mobis_Name)  { MobisSteering s;  EXPECT_STREQ("Mobis", s.getName()); }
TEST(SteeringTest, Bosch_Id)    { BoschSteering s;  EXPECT_EQ(1, s.getId()); }
TEST(SteeringTest, Mobis_Id)    { MobisSteering s;  EXPECT_EQ(2, s.getId()); }
```

---

## Phase 3 — Class Level (OCP 완성)

### 3-1. `CarConfig` — 부품 포인터 보유

```cpp
struct CarConfig {
    const ICarType*        carType        = nullptr;
    const IEngine*         engine         = nullptr;
    const IBrakeSystem*    brakeSystem    = nullptr;
    const ISteeringSystem* steeringSystem = nullptr;
};
```

### 3-2. `ICompatibilityRule` + 규칙 클래스 5종

```cpp
class ICompatibilityRule {
public:
    virtual ~ICompatibilityRule() = default;
    virtual bool        isFailed(const CarConfig& config) const = 0;
    virtual const char* getFailReason()                   const = 0;
};
```

| 클래스 | 규칙 (spec.md §3) |
|--------|-------------------|
| `SedanContinentalRule` | Sedan + Continental 불가 |
| `SuvToyotaRule`        | SUV + TOYOTA 불가 |
| `TruckWIARule`         | Truck + WIA 불가 |
| `TruckMandoRule`       | Truck + MANDO 불가 |
| `BoschMismatchRule`    | Bosch 브레이크 + 비Bosch 조향 불가 |

### Phase 3-2 Unit Tests — `Tests/Phase3_RuleTests.cpp`

각 규칙의 `isFailed()`를 FAIL 케이스 / PASS 케이스로 분리 검증.

```cpp
// --- SedanContinentalRule ---
TEST(SedanContinentalRuleTest, Fails_When_Sedan_Continental) {
    Sedan sedan; ContinentalBrake brake;
    CarConfig c; c.carType = &sedan; c.brakeSystem = &brake;
    EXPECT_TRUE(SedanContinentalRule().isFailed(c));
}
TEST(SedanContinentalRuleTest, Pass_When_SUV_Continental) {
    SUV suv; ContinentalBrake brake;
    CarConfig c; c.carType = &suv; c.brakeSystem = &brake;
    EXPECT_FALSE(SedanContinentalRule().isFailed(c));
}
TEST(SedanContinentalRuleTest, Pass_When_Sedan_Mando) {
    Sedan sedan; MandoBrake brake;
    CarConfig c; c.carType = &sedan; c.brakeSystem = &brake;
    EXPECT_FALSE(SedanContinentalRule().isFailed(c));
}

// --- SuvToyotaRule ---
TEST(SuvToyotaRuleTest, Fails_When_SUV_Toyota) {
    SUV suv; ToyotaEngine eng;
    CarConfig c; c.carType = &suv; c.engine = &eng;
    EXPECT_TRUE(SuvToyotaRule().isFailed(c));
}
TEST(SuvToyotaRuleTest, Pass_When_Sedan_Toyota) {
    Sedan sedan; ToyotaEngine eng;
    CarConfig c; c.carType = &sedan; c.engine = &eng;
    EXPECT_FALSE(SuvToyotaRule().isFailed(c));
}
TEST(SuvToyotaRuleTest, Pass_When_SUV_GM) {
    SUV suv; GMEngine eng;
    CarConfig c; c.carType = &suv; c.engine = &eng;
    EXPECT_FALSE(SuvToyotaRule().isFailed(c));
}

// --- TruckWIARule ---
TEST(TruckWIARuleTest, Fails_When_Truck_WIA) {
    Truck truck; WIAEngine eng;
    CarConfig c; c.carType = &truck; c.engine = &eng;
    EXPECT_TRUE(TruckWIARule().isFailed(c));
}
TEST(TruckWIARuleTest, Pass_When_Sedan_WIA) {
    Sedan sedan; WIAEngine eng;
    CarConfig c; c.carType = &sedan; c.engine = &eng;
    EXPECT_FALSE(TruckWIARule().isFailed(c));
}

// --- TruckMandoRule ---
TEST(TruckMandoRuleTest, Fails_When_Truck_Mando) {
    Truck truck; MandoBrake brake;
    CarConfig c; c.carType = &truck; c.brakeSystem = &brake;
    EXPECT_TRUE(TruckMandoRule().isFailed(c));
}
TEST(TruckMandoRuleTest, Pass_When_SUV_Mando) {
    SUV suv; MandoBrake brake;
    CarConfig c; c.carType = &suv; c.brakeSystem = &brake;
    EXPECT_FALSE(TruckMandoRule().isFailed(c));
}

// --- BoschMismatchRule ---
TEST(BoschMismatchRuleTest, Fails_When_BoschBrake_MobisSteering) {
    BoschBrake brake; MobisSteering steering;
    CarConfig c; c.brakeSystem = &brake; c.steeringSystem = &steering;
    EXPECT_TRUE(BoschMismatchRule().isFailed(c));
}
TEST(BoschMismatchRuleTest, Pass_When_BoschBrake_BoschSteering) {
    BoschBrake brake; BoschSteering steering;
    CarConfig c; c.brakeSystem = &brake; c.steeringSystem = &steering;
    EXPECT_FALSE(BoschMismatchRule().isFailed(c));
}
TEST(BoschMismatchRuleTest, Pass_When_MandoBrake_MobisSteering) {
    MandoBrake brake; MobisSteering steering;
    CarConfig c; c.brakeSystem = &brake; c.steeringSystem = &steering;
    EXPECT_FALSE(BoschMismatchRule().isFailed(c));
}

// getFailReason()은 비어있지 않아야 한다
TEST(AllRulesTest, FailReasonNotEmpty) {
    EXPECT_STRNE("", SedanContinentalRule().getFailReason());
    EXPECT_STRNE("", SuvToyotaRule().getFailReason());
    EXPECT_STRNE("", TruckWIARule().getFailReason());
    EXPECT_STRNE("", TruckMandoRule().getFailReason());
    EXPECT_STRNE("", BoschMismatchRule().getFailReason());
}
```

### 3-3. `CarValidator` — 규칙 목록 순회

```cpp
class CarValidator {
public:
    CarValidator();
    ValidationResult validate(const CarConfig& config) const;
private:
    std::vector<ICompatibilityRule*> m_rules;
};
```

### Phase 3-3 Unit Tests — `Tests/Phase3_CarValidatorTests.cpp`

**실제 규칙 사용 — 통합 검증 (spec.md §3 전체 커버):**

```cpp
// VALID: 문제없는 조합
TEST(CarValidatorTest, Valid_Sedan_GM_Mando_Bosch) {
    Sedan s; GMEngine e; MandoBrake b; BoschSteering st;
    CarConfig c{&s, &e, &b, &st};
    EXPECT_EQ(VALID, CarValidator().validate(c));
}
TEST(CarValidatorTest, Valid_Truck_GM_Bosch_Bosch) {
    Truck t; GMEngine e; BoschBrake b; BoschSteering st;
    CarConfig c{&t, &e, &b, &st};
    EXPECT_EQ(VALID, CarValidator().validate(c));
}

// FAIL: 규칙별 실패 케이스
TEST(CarValidatorTest, Fail_Sedan_Continental) {
    Sedan s; GMEngine e; ContinentalBrake b; BoschSteering st;
    CarConfig c{&s, &e, &b, &st};
    EXPECT_EQ(FAIL_SEDAN_CONTINENTAL, CarValidator().validate(c));
}
TEST(CarValidatorTest, Fail_SUV_Toyota) {
    SUV s; ToyotaEngine e; MandoBrake b; BoschSteering st;
    CarConfig c{&s, &e, &b, &st};
    EXPECT_EQ(FAIL_SUV_TOYOTA, CarValidator().validate(c));
}
TEST(CarValidatorTest, Fail_Truck_WIA) {
    Truck t; WIAEngine e; MandoBrake b; BoschSteering st;
    CarConfig c{&t, &e, &b, &st};
    EXPECT_EQ(FAIL_TRUCK_WIA, CarValidator().validate(c));
}
TEST(CarValidatorTest, Fail_Truck_Mando) {
    Truck t; GMEngine e; MandoBrake b; BoschSteering st;
    CarConfig c{&t, &e, &b, &st};
    EXPECT_EQ(FAIL_TRUCK_MANDO, CarValidator().validate(c));
}
TEST(CarValidatorTest, Fail_Bosch_Mismatch) {
    Sedan s; GMEngine e; BoschBrake b; MobisSteering st;
    CarConfig c{&s, &e, &b, &st};
    EXPECT_EQ(FAIL_BOSCH_MISMATCH, CarValidator().validate(c));
}
```

**Mock Rule 사용 — CarValidator 격리 테스트:**

```cpp
class MockCompatibilityRule : public ICompatibilityRule {
public:
    MOCK_METHOD(bool,        isFailed,      (const CarConfig&), (const, override));
    MOCK_METHOD(const char*, getFailReason, (),                 (const, override));
};

TEST(CarValidatorTest, CallsAllRulesOnValidConfig) {
    MockCompatibilityRule rule1, rule2;
    EXPECT_CALL(rule1, isFailed(_)).WillOnce(Return(false));
    EXPECT_CALL(rule2, isFailed(_)).WillOnce(Return(false));

    CarValidator validator({&rule1, &rule2});  // 외부 주입 생성자
    CarConfig c;
    EXPECT_EQ(VALID, validator.validate(c));
}

TEST(CarValidatorTest, StopsAtFirstFailedRule) {
    MockCompatibilityRule rule1, rule2;
    EXPECT_CALL(rule1, isFailed(_)).WillOnce(Return(true));
    EXPECT_CALL(rule1, getFailReason()).WillOnce(Return("rule1 fail"));
    EXPECT_CALL(rule2, isFailed(_)).Times(0);  // rule2는 호출되지 않아야 함

    CarValidator validator({&rule1, &rule2});
    CarConfig c;
    EXPECT_NE(VALID, validator.validate(c));
}
```

> `CarValidator`는 기본 생성자(내부 규칙 등록)와 외부 주입 생성자(테스트용) 두 가지를 제공한다.

### 3-4. `PartRegistry` — 메뉴 번호 → 부품 팩토리

### Phase 3-4 Unit Tests — `Tests/Phase3_PartRegistryTests.cpp`

```cpp
TEST(PartRegistryTest, GetCarType_1_ReturnsSedanType) {
    PartRegistry reg;
    EXPECT_EQ(1, reg.getCarType(1)->getId());
    EXPECT_STREQ("Sedan", reg.getCarType(1)->getName());
}
TEST(PartRegistryTest, GetCarType_2_ReturnsSUV) {
    EXPECT_EQ(2, PartRegistry().getCarType(2)->getId());
}
TEST(PartRegistryTest, GetCarType_3_ReturnsTruck) {
    EXPECT_EQ(3, PartRegistry().getCarType(3)->getId());
}
TEST(PartRegistryTest, GetCarType_InvalidId_ReturnsNull) {
    EXPECT_EQ(nullptr, PartRegistry().getCarType(0));
    EXPECT_EQ(nullptr, PartRegistry().getCarType(4));
}

TEST(PartRegistryTest, GetEngine_4_ReturnsNoEngine) {
    NoEngine* ne = dynamic_cast<NoEngine*>(
        const_cast<IEngine*>(PartRegistry().getEngine(4)));
    EXPECT_NE(nullptr, ne);
}
TEST(PartRegistryTest, GetEngine_InvalidId_ReturnsNull) {
    EXPECT_EQ(nullptr, PartRegistry().getEngine(0));
    EXPECT_EQ(nullptr, PartRegistry().getEngine(5));
}

TEST(PartRegistryTest, GetBrakeSystem_AllValid) {
    PartRegistry reg;
    EXPECT_EQ(1, reg.getBrakeSystem(1)->getId());
    EXPECT_EQ(2, reg.getBrakeSystem(2)->getId());
    EXPECT_EQ(3, reg.getBrakeSystem(3)->getId());
}
TEST(PartRegistryTest, GetSteering_AllValid) {
    PartRegistry reg;
    EXPECT_EQ(1, reg.getSteeringSystem(1)->getId());
    EXPECT_EQ(2, reg.getSteeringSystem(2)->getId());
}
```

### 3-5. `ICarPrinter` 인터페이스 + `CarPrinter` 구현

`CarAssembler`가 출력에 의존하지 않도록 `ICarPrinter` 인터페이스를 분리한다.
→ 테스트 시 `MockCarPrinter`로 대체 가능.

```cpp
class ICarPrinter {
public:
    virtual ~ICarPrinter() = default;
    virtual void printMenu(QuestionType step, const PartRegistry& reg) const = 0;
    virtual void printSelected(const ICarPart* part)                   const = 0;
    virtual void printRunResult(ValidationResult r, const CarConfig& c) const = 0;
    virtual void printTestResult(ValidationResult r,
                                 const ICompatibilityRule* failedRule) const = 0;
};

class CarPrinter : public ICarPrinter { /* ... */ };
```

### 3-6. `CarAssembler` — 의존성 주입 + 상태 머신

```cpp
class CarAssembler {
public:
    // 프로덕션 용: 기본 객체 내부 생성
    CarAssembler();
    // 테스트 용: Mock 주입
    CarAssembler(ICarPrinter* printer, CarValidator* validator, PartRegistry* registry);

    void run();

private:
    CarConfig      m_config;
    CarValidator*  m_validator;
    ICarPrinter*   m_printer;
    PartRegistry*  m_registry;
    QuestionType   m_step = CarType_Q;

    bool isInRange(int step, int answer) const;
    void processSelection(int answer);
};
```

### Phase 3-6 Unit Tests — `Tests/Phase3_CarAssemblerTests.cpp`

`MockCarPrinter`로 출력 부수효과를 제거하고, 상태 전이 로직만 테스트한다.

```cpp
class MockCarPrinter : public ICarPrinter {
public:
    MOCK_METHOD(void, printMenu,       (QuestionType, const PartRegistry&), (const, override));
    MOCK_METHOD(void, printSelected,   (const ICarPart*),                   (const, override));
    MOCK_METHOD(void, printRunResult,  (ValidationResult, const CarConfig&),(const, override));
    MOCK_METHOD(void, printTestResult, (ValidationResult, const ICompatibilityRule*), (const, override));
};

TEST(CarAssemblerTest, ProcessSelection_CarType_AdvancesToEngineStep) {
    MockCarPrinter printer;
    EXPECT_CALL(printer, printSelected(_)).Times(AnyNumber());
    CarAssembler assembler(&printer, nullptr, nullptr);

    assembler.selectPart(CarType_Q, 1);  // Sedan 선택
    EXPECT_EQ(Engine_Q, assembler.getCurrentStep());
}

TEST(CarAssemblerTest, ProcessSelection_BackNavigation_DecrementsStep) {
    MockCarPrinter printer;
    EXPECT_CALL(printer, printSelected(_)).Times(AnyNumber());
    CarAssembler assembler(&printer, nullptr, nullptr);

    assembler.selectPart(CarType_Q, 1);  // → Engine_Q
    assembler.goBack();                  // → CarType_Q
    EXPECT_EQ(CarType_Q, assembler.getCurrentStep());
}

TEST(CarAssemblerTest, RunTest_CallsPrintTestResult) {
    MockCarPrinter printer;
    MockCarValidator validator;
    EXPECT_CALL(validator, validate(_)).WillOnce(Return(VALID));
    EXPECT_CALL(printer, printTestResult(VALID, _)).Times(1);

    CarAssembler assembler(&printer, &validator, nullptr);
    assembler.triggerTest();
}

TEST(CarAssemblerTest, RunTest_FailCase_CallsPrintTestResultWithFail) {
    MockCarPrinter printer;
    MockCarValidator validator;
    EXPECT_CALL(validator, validate(_)).WillOnce(Return(FAIL_SEDAN_CONTINENTAL));
    EXPECT_CALL(printer, printTestResult(FAIL_SEDAN_CONTINENTAL, _)).Times(1);

    CarAssembler assembler(&printer, &validator, nullptr);
    assembler.triggerTest();
}
```

### 3-7. 파일 분리 및 최종 구조

```
car_assemble/
├── main.cpp
├── ICarPart.h
├── ICarType.h / IEngine.h / IBrakeSystem.h / ISteeringSystem.h
├── ICompatibilityRule.h
├── ICarPrinter.h
├── CarTypes/
│   ├── Sedan.h/.cpp
│   ├── SUV.h/.cpp
│   └── Truck.h/.cpp
├── Engines/
│   ├── GMEngine.h/.cpp
│   ├── ToyotaEngine.h/.cpp
│   ├── WIAEngine.h/.cpp
│   └── NoEngine.h/.cpp
├── BrakeSystems/
│   ├── MandoBrake.h/.cpp
│   ├── ContinentalBrake.h/.cpp
│   └── BoschBrake.h/.cpp
├── SteeringSystems/
│   ├── BoschSteering.h/.cpp
│   └── MobisSteering.h/.cpp
├── Rules/
│   ├── SedanContinentalRule.h/.cpp
│   ├── SuvToyotaRule.h/.cpp
│   ├── TruckWIARule.h/.cpp
│   ├── TruckMandoRule.h/.cpp
│   └── BoschMismatchRule.h/.cpp
├── CarConfig.h
├── CarValidator.h/.cpp
├── PartRegistry.h/.cpp
├── CarPrinter.h/.cpp
├── CarAssembler.h/.cpp
└── Tests/
    ├── Phase1_UtilTests.cpp
    ├── Phase2_PartTests.cpp
    ├── Phase3_RuleTests.cpp
    ├── Phase3_CarValidatorTests.cpp
    ├── Phase3_PartRegistryTests.cpp
    └── Phase3_CarAssemblerTests.cpp
```

---

## 실행 순서 요약

```
Phase 1 — Method Level + 유닛테스트
  1-1 ~ 1-6. 기존 방식 수정 (main.cpp 내부)
  T-1. Tests/Phase1_UtilTests.cpp  (isInRange 경계값 테스트 14개)

Phase 2 — Interface Level + 유닛테스트
  2-1. ICarPart
  2-2. ICarType + Sedan / SUV / Truck
  2-3. IEngine + GM / Toyota / WIA / NoEngine
  2-4. IBrakeSystem + Mando / Continental / Bosch
  2-5. ISteeringSystem + Bosch / Mobis
  T-2. Tests/Phase2_PartTests.cpp  (getName/getId/isSelected 22개)

Phase 3 — Class Level + 유닛테스트
  3-1. CarConfig
  3-2. ICompatibilityRule + 규칙 5종
  T-3a. Tests/Phase3_RuleTests.cpp          (규칙별 FAIL/PASS + getFailReason 21개)
  3-3. CarValidator (기본 + 외부 주입 생성자)
  T-3b. Tests/Phase3_CarValidatorTests.cpp  (통합 5종 + Mock 격리 2개)
  3-4. PartRegistry
  T-3c. Tests/Phase3_PartRegistryTests.cpp  (ID → 부품 매핑 11개)
  3-5. ICarPrinter 인터페이스 + CarPrinter 구현
  3-6. CarAssembler (의존성 주입 생성자 포함)
  T-3d. Tests/Phase3_CarAssemblerTests.cpp  (상태전이 + MockPrinter 4개)
  3-7. 파일 분리 + vcxproj 업데이트
```

> 각 T-* 단계는 해당 구현 단계 직후에 작성 및 통과 확인 후 다음 단계 진입.
> 모든 테스트는 `_DEBUG` 빌드의 `RUN_ALL_TESTS()`로 실행.

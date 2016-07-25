// Copyright 2016 the V8 project authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "src/builtins/builtins.h"
#include "src/builtins/builtins-utils.h"
#include "src/ic/handler-compiler.h"
#include "src/ic/ic.h"

namespace v8 {
namespace internal {

void Builtins::Generate_KeyedLoadIC_Megamorphic(MacroAssembler* masm) {
  KeyedLoadIC::GenerateMegamorphic(masm);
}

void Builtins::Generate_KeyedLoadIC_Miss(MacroAssembler* masm) {
  KeyedLoadIC::GenerateMiss(masm);
}
void Builtins::Generate_KeyedLoadIC_Slow(MacroAssembler* masm) {
  KeyedLoadIC::GenerateRuntimeGetProperty(masm);
}

void Builtins::Generate_KeyedStoreIC_Megamorphic(MacroAssembler* masm) {
  KeyedStoreIC::GenerateMegamorphic(masm, SLOPPY);
}

void Builtins::Generate_KeyedStoreIC_Megamorphic_Strict(MacroAssembler* masm) {
  KeyedStoreIC::GenerateMegamorphic(masm, STRICT);
}

void Builtins::Generate_KeyedStoreIC_Miss(MacroAssembler* masm) {
  KeyedStoreIC::GenerateMiss(masm);
}

void Builtins::Generate_KeyedStoreIC_Slow(MacroAssembler* masm) {
  ElementHandlerCompiler::GenerateStoreSlow(masm);
}

void Builtins::Generate_LoadGlobalIC_Miss(CodeStubAssembler* assembler) {
  typedef compiler::Node Node;

  Node* slot = assembler->Parameter(0);
  Node* vector = assembler->Parameter(1);
  Node* context = assembler->Parameter(2);

  assembler->TailCallRuntime(Runtime::kLoadGlobalIC_Miss, context, slot,
                             vector);
}

namespace {
void Generate_LoadGlobalIC_Slow(CodeStubAssembler* assembler, TypeofMode mode) {
  typedef compiler::Node Node;

  Node* slot = assembler->Parameter(0);
  Node* vector = assembler->Parameter(1);
  Node* context = assembler->Parameter(2);
  Node* typeof_mode = assembler->SmiConstant(Smi::FromInt(mode));

  assembler->TailCallRuntime(Runtime::kGetGlobal, context, slot, vector,
                             typeof_mode);
}
}  // anonymous namespace

void Builtins::Generate_LoadGlobalIC_SlowInsideTypeof(
    CodeStubAssembler* assembler) {
  Generate_LoadGlobalIC_Slow(assembler, INSIDE_TYPEOF);
}

void Builtins::Generate_LoadGlobalIC_SlowNotInsideTypeof(
    CodeStubAssembler* assembler) {
  Generate_LoadGlobalIC_Slow(assembler, NOT_INSIDE_TYPEOF);
}

void Builtins::Generate_LoadIC_Getter_ForDeopt(MacroAssembler* masm) {
  NamedLoadHandlerCompiler::GenerateLoadViaGetterForDeopt(masm);
}

void Builtins::Generate_LoadIC_Miss(CodeStubAssembler* assembler) {
  typedef compiler::Node Node;

  Node* receiver = assembler->Parameter(0);
  Node* name = assembler->Parameter(1);
  Node* slot = assembler->Parameter(2);
  Node* vector = assembler->Parameter(3);
  Node* context = assembler->Parameter(4);

  assembler->TailCallRuntime(Runtime::kLoadIC_Miss, context, receiver, name,
                             slot, vector);
}

void Builtins::Generate_LoadIC_Normal(MacroAssembler* masm) {
  LoadIC::GenerateNormal(masm);
}

void Builtins::Generate_LoadIC_Slow(CodeStubAssembler* assembler) {
  typedef compiler::Node Node;

  Node* receiver = assembler->Parameter(0);
  Node* name = assembler->Parameter(1);
  // Node* slot = assembler->Parameter(2);
  // Node* vector = assembler->Parameter(3);
  Node* context = assembler->Parameter(4);

  assembler->TailCallRuntime(Runtime::kGetProperty, context, receiver, name);
}

void Builtins::Generate_StoreIC_Miss(CodeStubAssembler* assembler) {
  typedef compiler::Node Node;

  Node* receiver = assembler->Parameter(0);
  Node* name = assembler->Parameter(1);
  Node* value = assembler->Parameter(2);
  Node* slot = assembler->Parameter(3);
  Node* vector = assembler->Parameter(4);
  Node* context = assembler->Parameter(5);

  assembler->TailCallRuntime(Runtime::kStoreIC_Miss, context, receiver, name,
                             value, slot, vector);
}

void Builtins::Generate_StoreIC_Normal(MacroAssembler* masm) {
  StoreIC::GenerateNormal(masm);
}

void Builtins::Generate_StoreIC_Setter_ForDeopt(MacroAssembler* masm) {
  NamedStoreHandlerCompiler::GenerateStoreViaSetterForDeopt(masm);
}

namespace {
void Generate_StoreIC_Slow(CodeStubAssembler* assembler,
                           LanguageMode language_mode) {
  typedef compiler::Node Node;

  Node* receiver = assembler->Parameter(0);
  Node* name = assembler->Parameter(1);
  Node* value = assembler->Parameter(2);
  // Node* slot = assembler->Parameter(3);
  // Node* vector = assembler->Parameter(4);
  Node* context = assembler->Parameter(5);
  Node* lang_mode = assembler->SmiConstant(Smi::FromInt(language_mode));

  // The slow case calls into the runtime to complete the store without causing
  // an IC miss that would otherwise cause a transition to the generic stub.
  assembler->TailCallRuntime(Runtime::kSetProperty, context, receiver, name,
                             value, lang_mode);
}
}  // anonymous namespace

void Builtins::Generate_StoreIC_SlowSloppy(CodeStubAssembler* assembler) {
  Generate_StoreIC_Slow(assembler, SLOPPY);
}

void Builtins::Generate_StoreIC_SlowStrict(CodeStubAssembler* assembler) {
  Generate_StoreIC_Slow(assembler, STRICT);
}

}  // namespace internal
}  // namespace v8

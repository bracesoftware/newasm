// Copyright (c) 2026 Brace Software Co.
// NewASM Virtual Machine and Toolchain

#define _NEWASM_CALL_FUNC() (newasm::funcHandler ? newasm::funcHandler() : "err")

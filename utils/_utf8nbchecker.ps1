function Get-FileEncoding {
    param([string]$Path)
    
    # 读取前4字节判断BOM
    $bytes = [byte[]](Get-Content -Path $Path -Encoding byte -ReadCount 4 -TotalCount 4)

    # UTF-8 BOM
    if ($bytes.Count -ge 3 -and $bytes[0] -eq 0xEF -and $bytes[1] -eq 0xBB -and $bytes[2] -eq 0xBF) {
        return "UTF-8 BOM"
    }
    # UTF-16 LE
    if ($bytes.Count -ge 2 -and $bytes[0] -eq 0xFF -and $bytes[1] -eq 0xFE) {
        return "UTF-16 LE"
    }
    # UTF-16 BE
    if ($bytes.Count -ge 2 -and $bytes[0] -eq 0xFE -and $bytes[1] -eq 0xFF) {
        return "UTF-16 BE"
    }

    # ===================== 修复部分：真正验证是否为 UTF-8 =====================
    try {
        # 尝试用 UTF-8 解码整个文件（无BOM）
        $content = Get-Content -Path $Path -Encoding UTF8 -Raw -ErrorAction Stop
        return "ASCII/UTF-8 无BOM"  # 能成功解码 = 一定是 UTF-8 no BOM
    }
    catch {
        # 解码失败 = 不是 UTF-8
        return "ANSI/GBK (非UTF-8)"
    }
}

# 扫描指定目录下的代码文件
Get-ChildItem -Path "../src" -Recurse -Include *.h,*.cpp,*.hpp,c | ForEach-Object {
    $enc = Get-FileEncoding $_.FullName
    if ($enc -ne "ASCII/UTF-8 无BOM") {
        Write-Host "[$enc] $($_.FullName)"
    }
}
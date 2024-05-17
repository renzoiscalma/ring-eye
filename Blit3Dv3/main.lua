local scores = {}
function get_max_scores()
    local file = 'Scores.dat'
    local lines = lines_from(file)
    local maxScore = 0

    for k,v in pairs(lines) do
        print('line[' .. k .. ']', v)
        scores[k] = v
        maxScore = tonumber(v) > maxScore and tonumber(v) or maxScore
    end
    return maxScore
end

function save_scores(score)
    print("\n")
    local idx = table.getn(scores)
    scores[idx+1] = score
    save_to_file()
end

function save_to_file()
    local file = io.open("Scores.dat", "w")
    if file then
        -- Write content to the file
        for _,k in ipairs(scores) do
            file:write(k.."\n")
        end
        -- Close the file
        file:close()
    else
        print("Error opening the file.")
    end
end


-- see if the file exists
function file_exists(file)
    local f = io.open(file, "rb")
    if f then f:close() end
    return f ~= nil
end

-- get all lines from a file, returns an empty 
-- list/table if the file does not exist
function lines_from(file)
    if not file_exists(file) then return {} end
    local lines = {}
    for line in io.lines(file) do 
        lines[#lines + 1] = line
    end
    return lines
end

